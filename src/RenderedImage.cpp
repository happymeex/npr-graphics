#include "RenderedImage.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

// CEL

void RenderedImage::DrawEdges(float edge_strength) {
    const auto beta_edges = diffuse_.GetEdges();

    // Multiply to get edged image
    const auto multiply_inverse = [edge_strength](glm::vec3 a, glm::vec3 b) {
        return a * (glm::vec3(1.0, 1.0, 1.0) - edge_strength * b);
    };

    final_ = final_.ApplyLayer(beta_edges, multiply_inverse);
}

// PIGMENT-BASED

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

// EDGE-BASED

glm::vec2 GetGradient(Image im, int x, int y) {
    auto w = im.GetWidth();
    auto h = im.GetHeight();

    float u = 0.0f;
    float v = 0.0f;
    
    if (x == 0) {
        u = glm::length(im.GetPixel(1, y) - im.GetPixel(0, y));
    } else if (x == w - 1) {
        u = glm::length(im.GetPixel(w-1, y) - im.GetPixel(w-2, y));
    } else {
        u = glm::length(im.GetPixel(x+1, y) - im.GetPixel(x-1, y)) / 2;
    }

    if (y == 0) {
        v = glm::length(im.GetPixel(x, 1) - im.GetPixel(x, 0));
    } else if (y == h - 1) {
        v = glm::length(im.GetPixel(x, h-1) - im.GetPixel(x, h-2));
    } else {
        v = glm::length(im.GetPixel(x, y+1) - im.GetPixel(x, y-1)) / 2;
    }

    if (u == 0 && v == 0) {
        return glm::vec2(0, 0);
    } else {
        return glm::normalize(glm::vec2(u, v));
    }
}



void RenderedImage::GapsAndOverlaps(int m, int p) {
    // edge image
    const auto e = diffuse_.GetEdges();
    // TODO: extended edge image, by applying a kernel of size 2m+1 on the edge image 
    const auto ee = e.GaussianBlur(m);
    // substrate color
    const auto cs = WATERCOLOR_PAPER_COLOR;

    Image result{width_, height_};
    
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            auto current = GetRGBD(x, y);
            result.SetPixel(x, y, current);
            // Check if not substrate color
            if (glm::vec3(current) != cs) {
                // Check if in extended edge
                if (ee.GetPixel(x, y)[0] > 0.2) {
                    // TODO: Get gradient by fetching neighbor pixels
                    auto g = GetGradient(ee, x, y);
                    // OVERLAPS
                    if (p > 0) {
                        for (int i = 0; i <= p; ++i) {
                            // Get neighboring color
                            int new_x = std::round(x + i*g[0]);
                            int new_y = std::round(y + i*g[1]);
                            auto cn = GetRGBD(new_x, new_y, false);
                            // Check for difference in RGBD space
                            if (glm::length(cn - current) > 0.5f) {
                                // If not substract, mix
                                if (glm::vec3(cn) != cs) {
                                    // Mix colors
                                    // TODO: RYB space conversion
                                    result.SetPixel(x, y, 0.5f * (cn + current));
                                }
                                break;
                            }
                            // Loop reached max -> direct edge
                            if (i == m) {
                                // Gradient did not converge to another color
                                int new_x = std::round(x - g[0]);
                                int new_y = std::round(y - g[1]);
                                auto cn = GetRGBD(new_x, new_y, false);
                                result.SetPixel(x, y, 0.5f * (cn + current));
                            }
                        }
                    }
                    // GAPS
                    if (p < 0) {
                        // Check if direct edge
                        if (e.GetPixel(x, y)[0] > 0.7) {
                            result.SetPixel(x, y, glm::vec4(cs, current[3]));
                        } else {
                            for (int i = 0; i <= -p; ++i) {
                                // Get neighboring color
                                int new_x = std::round(x + i*g[0]);
                                int new_y = std::round(y + i*g[1]);
                                auto cn = GetRGBD(new_x, new_y, false);
                                // Check for different in RGBD space
                                if (glm::length(cn - current) > 0.5) {
                                    result.SetPixel(x, y, glm::vec4(cs, 1.0f));
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            // Replace depth with alpha
            result.SetPixel(x, y, glm::vec4(glm::vec3(result.GetPixel(x, y)), final_.GetPixel(x, y)[3]));
        }
    }
    final_ = result;
}

// ABSTRACTION-BASED

void RenderedImage::Bleed(Image mask, int kernel_size, float depth_threshold) {
    const auto kernel_weights = Image::CalculateKernelWeights(kernel_size);

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