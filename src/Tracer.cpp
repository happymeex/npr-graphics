#include "Tracer.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Image Tracer::RenderOnce(RenderStyle style) {
    Image image(width_, height_);
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            float mapped_x = 2.0f * (float)x / (width_ - 1) - 1.0f;
            float mapped_y = 2.0f * (float)y / (height_ - 1) - 1.0f;
            Ray ray = camera_.GetRay(mapped_x, -mapped_y);
            HitRecord hit_record = HitRecord();
            glm::vec4 color =
                glm::vec4(TraceRay(ray, 0, hit_record, style).color, 1.0f);

            image.SetPixel(x, y, color);
        }
    }

    return image;
}

ImageInfo Tracer::RenderInfo(RenderStyle style) {
    Image color_image{width_, height_};
    Image beta_image{width_, height_};
    Image normal_image{width_, height_};
    Image depth_image{width_, height_};
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            float mapped_x = 2.0f * (float)x / (width_ - 1) - 1.0f;
            float mapped_y = 2.0f * (float)y / (height_ - 1) - 1.0f;
            Ray ray = camera_.GetRay(mapped_x, -mapped_y);
            HitRecord hit_record = HitRecord();

            auto pixel_info = TraceRay(ray, 0, hit_record, style);

            color_image.SetPixel(x, y, glm::vec4(pixel_info.color, 1.0f));
            beta_image.SetPixel(x, y, glm::vec4(pixel_info.diffuse_color, 1.0f));
            normal_image.SetPixel(x, y, glm::vec4(pixel_info.normal, 1.0f), false);
            depth_image.SetPixel(x, y, glm::vec4(pixel_info.depth, pixel_info.depth, pixel_info.depth, 1.0f), false);
        }
    }
    return {
        .color = color_image,
        .beta = beta_image,
        .normal = normal_image,
        .depth = depth_image
    };
}

void Tracer::Render(const Scene &scene, const std::string &out_file_name,
                    RenderStyle style) {
    scene_ = &scene;

    if (style == RenderStyle::Cel) {
        // Get the beta image to generate the edges
        const auto image_info = RenderInfo(RenderStyle::Cel);

        const auto beta_image = image_info.beta;
        const auto beta_edges = beta_image.GetEdges();

        // Render the cel image
        const auto cel_image = image_info.color;

        // Multiply to get edged image
        const auto multiply_inverse = [](glm::vec3 a, glm::vec3 b) {
            return a * (glm::vec3(1.0, 1.0, 1.0) - 0.5f * b);
        };
        const auto edged_image =
            cel_image.ApplyLayer(beta_edges, multiply_inverse);

        edged_image.SavePNG(out_file_name);
    } else if (style == RenderStyle::Real) {
        const auto result = RenderOnce(RenderStyle::Real);
        result.SavePNG(out_file_name);
    }
}

PixelInfo Tracer::TraceRay(const Ray &ray, int bounces, HitRecord &hit_record, RenderStyle style) {
    PixelInfo result;

    glm::vec3 diffuse_color(0.0f, 0.0f, 0.0f);
    glm::vec3 specular_color(0.0f, 0.0f, 0.0f);
    float shininess = 0.0f;
    bool any_hit = false;
    for (auto &obj : scene_->children) {
        bool hit = obj->Intersects(ray, 0.0f, hit_record);
        any_hit |= hit;
        if (hit) {
            auto material = obj->GetMaterial();
            diffuse_color = material.diffuse;
            specular_color = material.specular;
            shininess = material.shininess;
        }
    }
    if (!any_hit) {
        result.color = background_color_;
        result.diffuse_color = background_color_;
        result.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        result.depth = 0.0f;
        return result;
    }

    result.diffuse_color = diffuse_color;
    result.normal = hit_record.normal;
    result.depth = hit_record.time;

    if (style == RenderStyle::Beta) {
        result.color = diffuse_color;
        return result;
    }

    glm::vec3 color(0.f);
    glm::vec3 hit_pos = ray.origin + ray.direction * hit_record.time;
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

        // TODO: Shadows

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