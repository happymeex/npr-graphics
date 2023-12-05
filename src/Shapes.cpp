#include "Shapes.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

bool Sphere::Intersects(const Ray &ray, float t_min, HitRecord &hit_record) {
    glm::vec3 oc = ray.origin - (center_ + translation_);
    float a = glm::length2(ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::length2(oc) - radius_ * radius_;

    float d = b * b - 4.0f * a * c;

    if (d < 0.0f) {
        return false;
    }
    d = sqrt(d);
    float t_plus = (-b + d) / (2.0f * a);
    float t_minus = (-b - d) / (2.0f * a);

    float t;
    if (t_minus < t_min) {
        if (t_plus < t_min) {
            return false;
        } else {
            t = t_plus;
        }
    } else {
        t = t_minus;
    }

    if (t < hit_record.time) {
        hit_record.time = t;
        hit_record.normal = glm::normalize(oc + t * ray.direction);
        return true;
    }

    return false;
}

bool Triangle::Intersects(const Ray &ray, float t_min, HitRecord &record) {
    throw "Triangle intersection not implemented";
}

bool Plane::Intersects(const Ray &ray, float t_min, HitRecord &record) {
    glm::vec3 oc = ray.origin - translation_;
    float t = -(d_ + glm::dot(oc, normal_)) / glm::dot(ray.direction, normal_);
    if (t < t_min || t > record.time) {
        return false;
    }
    record.time = t;
    record.normal = normal_;
    return true;
}