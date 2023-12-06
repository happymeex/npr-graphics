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