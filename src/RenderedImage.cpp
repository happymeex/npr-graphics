#include "RenderedImage.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>

RenderedImage::RenderedImage(const Image &color, const Image &diffuse,
                             const Image &normal, const Image &depth,
                             const Image &density, const Image &final)
    : width_(final.GetWidth()), height_(final.GetHeight()), color_(color),
      diffuse_(diffuse), normal_(normal), depth_(depth), density_(density),
      paper_mask_(final.GetWidth(), final.GetHeight()), final_(final) {
    paper_ =
        std::unique_ptr<Paper>(new Paper("paper_uvh.txt", width_, height_, 45));
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            glm::vec3 paper_illumination = paper_->GetPaperIllumination(x, y);
            paper_mask_.SetPixel(x, y, glm::vec4(paper_illumination, 1.0f),
                                 false);
        }
    }
};

std::vector<float> CalculateKernelWeights(int n, float sigma = 1.0f) {
    float total = 0.0f;
    std::vector<float> result;
    for (int i = -n; i <= n; ++i) {
        float value = std::exp(-n * n / (2 * sigma * sigma));
        result.push_back(value);
        total += value;
    }
    for (auto it = result.begin(); it != result.end(); ++it) {
        *it = *it / total;
    }
    return result;
}

void RenderedImage::Bleed(Image mask, int kernel_size, float depth_threshold) {
    const auto kernel_weights = CalculateKernelWeights(kernel_size);

    Image bled_image{width_, height_};

    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            for (int i = -kernel_size; i <= kernel_size; ++i) {
                auto x_shift = std::min(std::max(x + i, 0), width_ - 1);
                // Check if source or destination pixels are bled
                if (mask.GetPixel(x, y)[0] > 0 ||
                    (mask.GetPixel(x_shift, y)[0] > 0)) {
                    // Initialize bleed and depth queries
                    auto bleed = false;
                    auto source_behind = false;
                    // Check if source pixel is behind
                    if (depth_.GetPixel(x, y)[0] - depth_threshold >
                        depth_.GetPixel(x_shift, y)[0]) {
                        source_behind = true;
                    }
                    // Check bleeding cases
                    if (mask.GetPixel(x_shift, y)[0] > 0 && source_behind) {
                        bleed = true;
                    } else if (mask.GetPixel(x, y)[0] > 0 && !source_behind) {
                        bleed = true;
                    }
                    // Bleed colors
                    if (bleed) {
                        // Add weighted destination color
                        bled_image.SetPixel(x, y,
                                            bled_image.GetPixel(x, y) +
                                                final_.GetPixel(x_shift, y) *
                                                    kernel_weights.at(i + 10));
                    } else {
                        // Add weighted source color
                        bled_image.SetPixel(x, y,
                                            bled_image.GetPixel(x, y) +
                                                final_.GetPixel(x, y) *
                                                    kernel_weights.at(i + 10));
                    }
                } else {
                    bled_image.SetPixel(x, y,
                                        bled_image.GetPixel(x, y) +
                                            final_.GetPixel(x, y) *
                                                kernel_weights.at(i + 10));
                }
            }
            // TODO: control image mask logic
            mask.SetPixel(x, y, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    final_ = bled_image;
}

void RenderedImage::DrawEdges(float edge_strength) {
    const auto beta_edges = diffuse_.GetEdges();

    // Multiply to get edged image
    const auto multiply_inverse = [edge_strength](glm::vec3 a, glm::vec3 b) {
        return a * (glm::vec3(1.0, 1.0, 1.0) - edge_strength * b);
    };

    final_ = final_.ApplyLayer(beta_edges, multiply_inverse);
}

void RenderedImage::ApplyDensity() {
    const auto apply_density = [this](glm::vec3 color,
                                      glm::vec3 density) -> glm::vec3 {
        glm::vec3 alpha = glm::vec3(1.f) + density;
        glm::vec3 color_pow = glm::pow(color, alpha);
        alpha = glm::clamp(alpha, 0.f, 1.f);
        return alpha * color_pow +
               (glm::vec3(1.f) - alpha) * WATERCOLOR_PAPER_COLOR;
    };
    final_ = final_.ApplyLayer(density_, apply_density);
}

void RenderedImage::ApplyPaperTexture() {
    const auto apply_paper_texture =
        [this](glm::vec3 color, glm::vec3 paper_illumination) -> glm::vec3 {
        return color * paper_illumination;
    };
    final_ = final_.ApplyLayer(paper_mask_, apply_paper_texture);
}