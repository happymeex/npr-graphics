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
    /**
     * For watercolor effects. Returns the pigment density of the object at some
     * hit point on its surface, as a function of the 2D image position and the
     * hit point in world coordinates.
     *
     * @param img_pos 2D image space position, coordinates range from -1 to 1
     * @param position The hit point in world coordinates
     */
    virtual float GetDensity(const glm::vec2 &img_pos,
                             const glm::vec3 &position) const;
    void SetDensitySeed(int seed) { pigment_density_perlin_.reseed(seed); }
    void SetDensityStrength(float strength) { density_strength_ = strength; }
    void SetDensityScale(float scale) { density_scale_ = scale; }

  protected:
    Material material_;
    glm::vec3 translation_;
    float density_strength_ = 1.0f;
    float density_scale_ = 1.0f;

    siv::PerlinNoise pigment_density_perlin_;
};

#endif