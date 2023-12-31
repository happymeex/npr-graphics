#ifndef RENDERED_IMAGE
#define RENDERED_IMAGE

#include "Camera.hpp"
#include "HitRecord.hpp"
#include "Image.hpp"
#include "Paper.hpp"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include <string>

struct PixelInfo {
    glm::vec3 color;
    glm::vec3 diffuse_color;
    glm::vec3 normal;
    float pigment_density; // in [-1, 1]
    float depth;
};

class RenderedImage {
  public:
    RenderedImage(const Image &color, const Image &diffuse, const Image &normal,
                  const Image &depth, const Image &density, const Image &final);

    void GapsAndOverlaps(int m, int p);

    /**
     * Bleeds the image based on the provided mask, kernel size, and depth
     * threshold.
     *
     * @param mask The mask image used to determine which pixels to bleed.
     * @param kernel_size The size of the kernel used for bleeding.
     * @param depth_threshold The threshold for determining if source pixels are
     * behind destination pixels.
     *
     * @throws None
     */
    void Bleed(Image mask, int kernel_size = 5, float depth_threshold = 0.01f);

    void DrawEdges(float edge_strength = 1.0f);

    void DrawDarkenedEdges(float edge_strength = 1.0f, int kernel_size = 5);

    void ApplyDensity();

    void ApplyPaperTexture();

    const Image GetColor() const { return color_; }

    const Image GetDiffuse() const { return diffuse_; }

    const Image GetNormal() const { return normal_; }

    const Image GetDepth() const { return depth_; }

    const Image GetFinal() const { return final_; }

    const Image GetDensity() const { return density_; }

  private:
    int width_;
    int height_;
    float min_depth_;
    const Image color_;
    const Image diffuse_;
    const Image normal_;
    const Image depth_;
    const Image density_;
    Image paper_mask_;
    Image final_;
    std::unique_ptr<Paper> paper_;
    glm::vec3 WATERCOLOR_PAPER_COLOR = glm::vec3(.95f, .95f, .90f);

    glm::vec3 ApplyPigmentTurbulence(glm::vec3 color, glm::vec3 density);

    // Red Green Blue Depth
    glm::vec4 GetRGBD(int x, int y, bool strict = true) const {
      return glm::vec4(glm::vec3(final_.GetPixel(x, y, strict)), depth_.GetPixel(x, y, strict)[0]);
    }
};

#endif