#include "Image.hpp"
#include <iostream>
#include <lodepng.h>

void Image::SetPixel(int x, int y, glm::vec4 color) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw "Image SetPixel: index out of range";
    }
    data_[y * width_ + x] = color;
}
void Image::SavePNG(const std::string &file_name) {
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
