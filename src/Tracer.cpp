#include "Tracer.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Image Tracer::RenderOnce(RenderStyle style) {
    Image image(width_, height_);
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            float mapped_x = 2.0f * (float)x / (width_ - 1) - 1.0f;
            float mapped_y = 2.0f * (float)y / (height_ - 1) - 1.0f;
            glm::vec2 img_pos = glm::vec2(mapped_x, -mapped_y);
            Ray ray = camera_.GetRay(mapped_x, -mapped_y);
            HitRecord hit_record = HitRecord();
            glm::vec4 color = glm::vec4(
                TraceRay(img_pos, ray, 0, hit_record, style).color, 1.0f);

            image.SetPixel(x, y, color);
        }
    }

    return image;
}

RenderedImage Tracer::RenderInfo(RenderStyle style) {
    Image color_image{width_, height_};
    Image beta_image{width_, height_};
    Image normal_image{width_, height_};
    Image depth_image{width_, height_};
    Image density_image{width_, height_};
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            float mapped_x = 2.0f * (float)x / (width_ - 1) - 1.0f;
            float mapped_y = 2.0f * (float)y / (height_ - 1) - 1.0f;
            glm::vec2 img_pos = glm::vec2(mapped_x, -mapped_y);
            Ray ray = camera_.GetRay(mapped_x, -mapped_y);
            HitRecord hit_record = HitRecord();

            auto pixel_info = TraceRay(img_pos, ray, 0, hit_record, style);

            color_image.SetPixel(x, y, glm::vec4(pixel_info.color, 1.0f));
            beta_image.SetPixel(x, y,
                                glm::vec4(pixel_info.diffuse_color, 1.0f));
            normal_image.SetPixel(x, y, glm::vec4(pixel_info.normal, 1.0f),
                                  false);
            depth_image.SetPixel(x, y,
                                 glm::vec4(pixel_info.depth, pixel_info.depth,
                                           pixel_info.depth, 1.0f),
                                 false);
            density_image.SetPixel(
                x, y, glm::vec4(glm::vec3(pixel_info.pigment_density), 1.0f),
                false);
        }
    }

    return {color_image, beta_image,    normal_image,
            depth_image, density_image, color_image};
}

void Tracer::Render(const Scene &scene, const std::string &out_file_name,
                    RenderStyle style) {
    scene_ = &scene;

    if (style == RenderStyle::Real) {
        const auto result = RenderOnce(RenderStyle::Real);
        result.SavePNG(out_file_name);
    } else if (style == RenderStyle::Cel) {
        auto rendered_image = RenderInfo(RenderStyle::Cel);
        rendered_image.DrawEdges(0.5f);
        rendered_image.GetFinal().SavePNG(out_file_name);
    } else if (style == RenderStyle::Watercolor) {
        // Start with the rendered Cel image (can change to Real if appropriate)
        auto rendered_image = RenderInfo(RenderStyle::Real);

        // TODO: Implement other edge-based effects
        // rendered_image.DrawEdges(0.5f);
        rendered_image.DrawDarkenedEdges(20.0f, 5);
        // rendered_image.GapsAndOverlaps(1, 1);

        // TODO: Implement pigment-based effects
        rendered_image.ApplyDensity();

        // TODO: Implement substrate-based effects
        rendered_image.ApplyPaperTexture();

        // TODO: Add a better control mask
        Image mask{width_, height_};
        for (int y = 0; y < height_; y++) {
            for (int x = 0; x < width_; x++) {
                if ((x % 50 < 25 && y % 50 < 25) ||
                    (x % 50 >= 25 && y % 50 >= 25)) {
                    mask.SetPixel(x, y, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                }
            }
        }
        rendered_image.Bleed(mask, 3);

        rendered_image.GetFinal().SavePNG(out_file_name);
    }
}

PixelInfo Tracer::TraceRay(const glm::vec2 &img_pos, const Ray &ray,
                           int bounces, HitRecord &hit_record,
                           RenderStyle style) {
    PixelInfo result;
    result.pigment_density = 0.0f;

    glm::vec3 diffuse_color(0.0f, 0.0f, 0.0f);
    glm::vec3 specular_color(0.0f, 0.0f, 0.0f);
    float shininess = 0.0f;
    bool any_hit = false;
    glm::vec3 hit_pos(0.0f, 0.0f, 0.0f);
    for (auto &obj : scene_->children) {
        bool hit = obj->Intersects(ray, 0.0f, hit_record);
        any_hit |= hit;
        if (hit) {
            auto material = obj->GetMaterial();
            diffuse_color = material.diffuse;
            specular_color = material.specular;
            shininess = material.shininess;
            hit_pos = ray.origin + ray.direction * hit_record.time;
            result.pigment_density = obj->GetDensity(img_pos, hit_pos);
        }
    }
    if (!any_hit) {
        result.color = background_color_;
        result.diffuse_color = background_color_;
        result.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        result.depth = std::numeric_limits<float>::max();
        result.pigment_density = scene_->GetDensity(img_pos);
        return result;
    }

    result.diffuse_color = diffuse_color;
    result.normal = hit_record.normal;
    result.depth = hit_record.time + 0.01f;

    if (style == RenderStyle::Beta) {
        result.color = diffuse_color;
        return result;
    }

    glm::vec3 color(0.f);
    for (const auto &light : scene_->lights) {
        glm::vec3 dir_to_light;
        glm::vec3 light_intensity;
        float dist_to_light;
        light->GetIllumination(hit_pos, dir_to_light, light_intensity,
                               dist_to_light);
        if (light->GetType() == LightType::AMBIENT) {
            color += light_intensity * diffuse_color;
            continue;
        }

        if (IsShadowed(hit_pos, dir_to_light, dist_to_light)) {
            continue;
        }

        float diffuse_scalar =
            glm::max(0.0f, glm::dot(hit_record.normal, dir_to_light));
        if (style == RenderStyle::Cel) {
            if (diffuse_scalar > 0.3) {
                diffuse_scalar = 0.8;
            } else {
                diffuse_scalar = 0.1;
            }
        }
        color += light_intensity * diffuse_scalar * diffuse_color;

        // calculate specular shading
        glm::vec3 view_direction = glm::normalize(ray.origin - hit_pos);
        glm::vec3 reflection = glm::reflect(-dir_to_light, hit_record.normal);
        float specular_scalar = glm::pow(
            glm::max(0.0f, glm::dot(reflection, view_direction)), shininess);
        if (style == RenderStyle::Cel) {
            if (specular_scalar > 0.3) {
                specular_scalar = 0.8;
            } else {
                specular_scalar = 0.1;
            }
        }
        color += specular_scalar * light_intensity * specular_color;
    }

    result.color = color;
    return result;
}

bool Tracer::IsShadowed(const glm::vec3 &hit_pos, const glm::vec3 &dir_to_light,
                        const float dist_to_light) const {
    float eps = 0.001f;
    HitRecord record;
    record.time = dist_to_light;
    Ray shadow_ray{hit_pos + eps * dir_to_light, dir_to_light};
    for (auto &obj : scene_->children) {
        if (obj->Intersects(shadow_ray, 0.0f, record)) {
            return true;
        }
    }
    return false;
}