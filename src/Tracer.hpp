#ifndef TRACER
#define TRACER

#include "Camera.hpp"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include <string>

class Tracer {
  public:
    Tracer(const CameraSpec &camera_spec, int width, int height,
           const glm::vec3 &background_color, int max_bounces)
        : camera_(camera_spec), width_(width), height_(height),
          background_color_(background_color), max_bounces_(max_bounces){};
    void Render(const Scene &scene, const std::string &out_file_name);

  private:
    Camera camera_;
    int width_;
    int height_;
    glm::vec3 background_color_;
    int max_bounces_;
};

#endif