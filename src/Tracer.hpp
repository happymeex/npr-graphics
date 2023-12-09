#ifndef TRACER
#define TRACER

#include "Camera.hpp"
#include "HitRecord.hpp"
#include "Image.hpp"
#include "RenderedImage.hpp"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include <string>

enum class RenderStyle {
    Beta,
    Cel,
    Real,
    Watercolor,
};

class Tracer {
  public:
    Tracer(const CameraSpec &camera_spec, int width, int height,
           const glm::vec3 &background_color, int max_bounces)
        : camera_(camera_spec), width_(width), height_(height),
          background_color_(background_color), max_bounces_(max_bounces){};
    void Render(const Scene &scene, const std::string &out_file_name,
                RenderStyle style = RenderStyle::Real);

  private:
    Image RenderOnce(RenderStyle style);
    RenderedImage RenderInfo(RenderStyle style);

    Camera camera_;
    int width_;
    int height_;
    glm::vec3 background_color_;
    int max_bounces_;
    const Scene *scene_;

    PixelInfo TraceRay(int x, int y, const Ray &ray, int bounces,
                       HitRecord &hit_record, RenderStyle style);

    bool IsShadowed(const glm::vec3 &hit_pos, const glm::vec3 &dir_to_light,
                    const float dist_to_light) const;
};

#endif