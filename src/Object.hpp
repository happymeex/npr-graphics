#ifndef OBJECT
#define OBJECT

#include "BVH.hpp"
#include "Hittable.hpp"
#include "Mesh.hpp"
#include "Ray.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Object : public Hittable {
  public:
    Object(std::unique_ptr<Mesh> mesh)
        : mesh_(std::move(mesh)), bvh_(mesh_->triangles) {
        bvh_.ConstructBVH();
    };
    bool Intersects(const Ray &ray, float t_min, HitRecord &record) override;

  private:
    std::unique_ptr<Mesh> mesh_;
    BVHObject bvh_;
};

#endif