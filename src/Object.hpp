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
    Object(std::unique_ptr<NPR::Mesh> mesh) : mesh_(std::move(mesh)){};
    void Translate(glm::vec3 delta);

  private:
    glm::vec3 position_;
    std::unique_ptr<NPR::Mesh> mesh_;
};

#endif