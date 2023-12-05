#ifndef OBJECT
#define OBJECT

#include "Hittable.hpp"
#include "Mesh.hpp"
#include "Ray.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Object : public Hittable {
  public:
    Object(std::unique_ptr<Mesh> mesh) : mesh_(std::move(mesh)){};
    bool Intersects(const Ray &ray, float t_min, HitRecord &record) override;

  private:
    std::unique_ptr<Mesh> mesh_;
};

#endif