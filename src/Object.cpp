#include "Object.hpp"

bool Object::Intersects(const Ray &ray, float t_min, HitRecord &record) {
    // check intersection with triangles in mesh
    Ray translated_ray = ray;
    translated_ray.origin -= translation_;
    // bool any_hit = false;
    // for (auto &triangle : mesh_->triangles) {
    //     bool hit = triangle.Intersects(translated_ray, t_min, record);
    //     any_hit |= hit;
    // }
    // return any_hit;
    return bvh_.Intersects(translated_ray, t_min, record);
}

float Object::GetDensity(const glm::vec2 &img_pos,
                         const glm::vec3 &position) const {
    const BoundingBox &bbox = bvh_.GetBoundingBox();
    float scale = 1.5f;
    float x = 2.f * (position.x - bbox.min.x) / (bbox.max.x - bbox.min.x) - 1.f;
    float y = 2.f * (position.y - bbox.min.y) / (bbox.max.y - bbox.min.y) - 1.f;
    float z = 2.f * (position.z - bbox.min.z) / (bbox.max.z - bbox.min.z) - 1.f;
    x = scale * density_scale_ * x;
    y = scale * density_scale_ * y;
    z = scale * density_scale_ * z;
    float density =
        (float)(pigment_density_perlin_.normalizedOctave3D(x, y, z, 1));

    return 0.5f * density_strength_ * density;
}