#ifndef NODE
#define NODE

#include "Hittable.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include <memory>
#include <vector>

class Scene {
  public:
    Scene();
    void AddObject(std::unique_ptr<Hittable> obj);
    std::vector<std::unique_ptr<Hittable>> children;
};
#endif