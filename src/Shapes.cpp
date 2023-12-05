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
    glm::vec3 oc = ray.origin - translation_;
    glm::mat3 coeffs(1.f);
    coeffs[0] = positions_[0] - positions_[1];
    coeffs[1] = positions_[0] - positions_[2];
    coeffs[2] = ray.direction;
    glm::vec3 beta_gamma_t = glm::inverse(coeffs) * (positions_[0] - oc);
    float beta = beta_gamma_t[0];
    float gamma = beta_gamma_t[1];
    float t = beta_gamma_t[2];
    if (beta < 0 || gamma < 0 || beta + gamma > 1 || t < t_min ||
        t > record.time) {
        return false;
    }
    record.time = t;
    record.normal = (1 - beta - gamma) * normals_[0] + beta * normals_[1] +
                    gamma * normals_[2];
    return true;
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