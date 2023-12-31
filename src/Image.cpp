#include "Image.hpp"
#include <iostream>
#include <lodepng.h>

const glm::vec4 &Image::GetPixel(int x, int y, bool strict) const {
    if (!strict) {
        x = std::max(std::min(x, width_ - 1), 0);
        y = std::max(std::min(y, width_ - 1), 0);
    }
    if (x < width_ && y < height_) {
        return data_[y * width_ + x];
    } else {
        throw std::runtime_error(
            "Unable to get a pixel outside of image range.");
    }
}
void Image::SetPixel(int x, int y, glm::vec4 color, bool clip) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::runtime_error("Image SetPixel: index out of range");
    }
    if (clip) {
        data_[y * width_ + x] = glm::clamp(color, 0.0f, 1.0f);
    } else {
        data_[y * width_ + x] = color;
    }
}
void Image::SavePNG(const std::string &file_name) const {
    std::cout << "Saving image to " << file_name << std::endl;
    std::vector<unsigned char> data(width_ * height_ * 4);
    for (int i = 0; i < width_ * height_; i++) {
        data[4 * i] = (unsigned int)(std::min(1.0f, data_[i].r) * 255.0);
        data[4 * i + 1] = (unsigned int)(std::min(1.0f, data_[i].g) * 255.0);
        data[4 * i + 2] = (unsigned int)(std::min(1.0f, data_[i].b) * 255.0);
        data[4 * i + 3] = (unsigned int)(data_[i].a * 255.0);
    }
    unsigned int error =
        lodepng::encode("out/" + file_name, data, width_, height_);
    if (error) {
        std::cout << "Error: " << lodepng_error_text(error) << std::endl;
    }
}

// Image processing functions

/**
 * Applies a layer to the image using a given blending function.
 *
 * @param image2 The image to blend with.
 * @param function The blending function that takes two RGB colors and returns a
 * blended color.
 *
 * @return The resulting image after applying the layer.
 *
 * @throws std::invalid_argument if the size of image2 is different from the
 * current image.
 */
Image Image::ApplyLayer(
    const Image &image2,
    const std::function<glm::vec3(glm::vec3, glm::vec3)> &function) const {
    if (image2.GetWidth() != width_ || image2.GetHeight() != height_) {
        throw std::invalid_argument("Multiplying differently sized images");
    }

    Image result{width_, height_};
    for (int x = 0; x < width_; ++x) {
        for (int y = 0; y < height_; ++y) {
            auto p1 = GetPixel(x, y);
            auto p2 = image2.GetPixel(x, y);
            auto blended = function(glm::vec3(p1), glm::vec3(p2));

            auto alpha1 = p1[3];
            auto alpha2 = p2[3];

            auto new_pixel = (1 - alpha2) * glm::vec3(p1) + alpha2 * blended;
            result.SetPixel(x, y, glm::vec4(new_pixel, alpha1));
        }
    }
    return result;
}

Image Image::ApplyFilter(const std::vector<std::vector<float>> &filter,
                         bool on_transparency, bool clip) const {

    int n = filter.size();

    if (n % 2 != 1) {
        throw std::invalid_argument("Filter must be a square of odd size");
    }

    int m = (n - 1) / 2;

    Image result{width_, height_};

    float normalizer = 0.0f;

    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            normalizer += filter.at(x).at(y);
        }
    }

    if (normalizer == 0) {
        normalizer = 1.0f;
    }
    normalizer = 1.0f / normalizer;

    int upper_bound = static_cast<int>(width_) - 1;
    int lower_bound = static_cast<int>(height_) - 1;

    for (int x = 0; x < width_; ++x) {
        for (int y = 0; y < height_; ++y) {
            // Filter applied to transparency as well
            if (on_transparency) {
                glm::vec4 pixel{0, 0, 0, 0};
                for (int i = -m; i <= m; ++i) {
                    for (int j = -m; j <= m; ++j) {
                        auto true_x = std::min(std::max(0, x + i), upper_bound);
                        auto true_y = std::min(std::max(0, y + j), lower_bound);
                        pixel += filter.at(i + m).at(j + m) *
                                 GetPixel(true_x, true_y);
                    }
                }
                result.SetPixel(x, y, normalizer * pixel);
            } else {
                glm::vec3 pixel{0, 0, 0};
                auto alpha = GetPixel(x, y)[3];
                for (int i = -m; i <= m; ++i) {
                    for (int j = -m; j <= m; ++j) {
                        auto true_x = std::min(std::max(0, x + i), upper_bound);
                        auto true_y = std::min(std::max(0, y + j), lower_bound);
                        pixel += filter.at(i + m).at(j + m) *
                                 glm::vec3(GetPixel(true_x, true_y));
                    }
                }
                result.SetPixel(x, y, glm::vec4(normalizer * pixel, alpha),
                                clip);
            }
        }
    }
    return result;
}

Image Image::GetEdges() const {
    const auto magnitude = [](glm::vec3 a, glm::vec3 b) {
        float val = std::sqrt(glm::dot(a, a) + glm::dot(b, b));
        return glm::vec3{val, val, val};
    };

    return ApplyFilter(GX, false, false)
        .ApplyLayer(ApplyFilter(GY, false, false), magnitude);
}

Image Image::GaussianBlur(int m, float sigma) const {
    auto weights_1d = CalculateKernelWeights(m, sigma);
    std::vector<std::vector<float>> weights_2d;

    for (size_t i = 0; i != weights_1d.size(); ++i) {
        weights_2d.push_back({});
        for (size_t j = 0; j != weights_1d.size(); ++j) {
            weights_2d.at(i).push_back(weights_1d.at(i) * weights_1d.at(j));
        }
    }

    return ApplyFilter(weights_2d);
}
