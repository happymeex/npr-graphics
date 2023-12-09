#ifndef NODE
#define NODE

#include "Hittable.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include <PerlinNoise.hpp>
#include <memory>
#include <vector>

class Scene {
  public:
    Scene();
    void AddObject(std::unique_ptr<Hittable> obj);
    void AddLight(std::unique_ptr<Light> light);
    std::vector<std::unique_ptr<Hittable>> children;
    std::vector<std::unique_ptr<Light>> lights;
    float GetDensity(const glm::vec2 &img_pos) const;

  private:
    siv::PerlinNoise pigment_density_perlin_;
};
#endif