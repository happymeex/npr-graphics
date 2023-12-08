#ifndef HITTABLE
#define HITTABLE

#include "HitRecord.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include <PerlinNoise.hpp>

class Hittable {
  public:
    Hittable();
    virtual ~Hittable() = default;
    /**
     * Returns true if the ray intersects with the object at a time greater than
     * t_min. Updates the hit record if the ray intersects with the object.
     *
     * @param ray The ray to intersect with the object. Assumed to be in world
     * coordinates.
     * @param t_min The minimum time to intersect with the object.
     * @param hit_record The hit record to update if the ray intersects with
     */
    virtual bool Intersects(const Ray &ray, float t_min,
                            HitRecord &hit_record) = 0;
    void SetMaterial(Material &material) { material_ = material; }
    Material GetMaterial() { return material_; }
    void Translate(glm::vec3 translation) { translation_ += translation; }
    virtual float GetDensity(glm::vec3 position) const;

  protected:
    Material material_;
    glm::vec3 translation_;

    siv::PerlinNoise pigment_density_perlin_;
};

#endif