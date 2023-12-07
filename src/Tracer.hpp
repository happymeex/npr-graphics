#ifndef TRACER
#define TRACER

#include "Camera.hpp"
#include "HitRecord.hpp"
#include "Scene.hpp"
#include "Image.hpp"
#include <glm/glm.hpp>
#include <string>

enum class RenderStyle
{
  Beta,
  Cel,
  Real,
  Watercolor,
};

struct PixelInfo {
  glm::vec3 color;
  glm::vec3 diffuse_color;
  glm::vec3 normal;
  float depth;
};

struct ImageInfo {
  Image color;
  Image beta;
  Image normal;
  Image depth;
};

class Tracer
{
public:
  Tracer(const CameraSpec &camera_spec, int width, int height,
         const glm::vec3 &background_color, int max_bounces)
      : camera_(camera_spec), width_(width), height_(height),
        background_color_(background_color), max_bounces_(max_bounces){};
  void Render(const Scene &scene, const std::string &out_file_name, RenderStyle style = RenderStyle::Real);

private:
  Image RenderOnce(RenderStyle style);
  ImageInfo RenderInfo(RenderStyle style);

  Camera camera_;
  int width_;
  int height_;
  glm::vec3 background_color_;
  int max_bounces_;
  const Scene *scene_;

  PixelInfo TraceRay(const Ray &ray, int bounces, HitRecord &hit_record, RenderStyle style);
};

#endif