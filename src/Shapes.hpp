#ifndef SHAPES
#define SHAPES

#include "Hittable.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include <vector>

class Sphere : public Hittable {
  public:
    Sphere(glm::vec3 position, float radius)
        : position_(position), radius_(radius){};
    bool Intersects(const Ray &ray, float t_min, HitRecord &record) override;

  private:
    glm::vec3 position_;
    float radius_;
};

class Triangle : public Hittable {
  public:
    Triangle(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2,
             const glm::vec3 &n0, const glm::vec3 &n1, const glm::vec3 &n2)
        : positions_({p0, p1, p2}), normals_({n0, n1, n2}) {}

    Triangle(const std::vector<glm::vec3> &positions,
             const std::vector<glm::vec3> &normals)
        : positions_(positions), normals_(normals) {}
    bool Intersects(const Ray &ray, float t_min, HitRecord &record) override;

  private:
    std::vector<glm::vec3> positions_;
    std::vector<glm::vec3> normals_;
};

class Plane : public Hittable {
  public:
    Plane(const glm::vec3 &normal, float d) : normal_(normal), d_(d){};
    bool Intersects(const Ray &ray, float t_min, HitRecord &record) override;

  private:
    glm::vec3 normal_;
    float d_;
};

#endif