#include "Image.hpp"
#include <iostream>
#include <lodepng.h>

const glm::vec4 &Image::GetPixel(int x, int y) const {
    if (x < width_ && y < height_) {
        return data_[y * width_ + x];
    } else {
        throw std::runtime_error(
            "Unable to get a pixel outside of image range.");
    }
}
void Image::SetPixel(int x, int y, glm::vec4 color) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw "Image SetPixel: index out of range";
    }
    color.r = std::min(1.0f, color.r);
    color.g = std::min(1.0f, color.g);
    color.b = std::min(1.0f, color.b);
    data_[y * width_ + x] = color;
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
