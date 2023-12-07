#ifndef RENDERED_IMAGE
#define RENDERED_IMAGE

#include "Camera.hpp"
#include "HitRecord.hpp"
#include "Scene.hpp"
#include "Image.hpp"
#include <glm/glm.hpp>
#include <string>

struct PixelInfo {
  glm::vec3 color;
  glm::vec3 diffuse_color;
  glm::vec3 normal;
  float depth;
};

class RenderedImage
{
public:
  RenderedImage(const Image& color, const Image& diffuse, const Image& normal, const Image& depth, const Image& final) : color_(color), diffuse_(diffuse), normal_(normal), depth_(depth), final_(final) {};

  void Bleed(std::vector<float> kernel_weights);
  
  void DrawEdges(float edge_strength = 1.0f);

  const Image GetColor() const {
    return color_;
  }

  const Image GetDiffuse() const {
    return diffuse_;
  }

  const Image GetNormal() const {
    return normal_;
  }

  const Image GetDepth() const {
    return depth_;
  }

  const Image GetFinal() const {
    return final_;
  }

private:
  const Image color_;
  const Image diffuse_;
  const Image normal_;
  const Image depth_;
  Image final_;
};

#endif