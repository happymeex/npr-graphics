#ifndef HITTABLE
#define HITTABLE

#include "HitRecord.hpp"
#include "Material.hpp"
#include "Ray.hpp"

class Hittable {
  public:
    virtual ~Hittable() {}
    virtual bool Intersects(const Ray &ray, float t_min,
                            HitRecord &hit_record) = 0;
    void SetMaterial(Material &material) { material_ = material; }
    Material GetMaterial() { return material_; }

  protected:
    Material material_;
};

#endif