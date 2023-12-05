#include "Object.hpp"

bool Object::Intersects(const Ray &ray, float t_min, HitRecord &record) {
    // check intersection with triangles in mesh
    Ray translated_ray = ray;
    translated_ray.origin -= translation_;
    for (auto &triangle : mesh_->triangles) {
        if (triangle.Intersects(translated_ray, t_min, record)) {
            return true;
        }
    }
    return false;
}