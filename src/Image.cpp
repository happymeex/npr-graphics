#include "Image.hpp"
#include <lodepng.h>

void Image::SetPixel(int x, int y, glm::vec4 color) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw "Image SetPixel: index out of range";
    }
    data_[y * width_ + x] = color;
}
void Image::SavePNG(const std::string &file_name) {
    printf("Saving image to %s\n", file_name.c_str());
    std::vector<unsigned char> data(width_ * height_ * 4);
    for (int i = 0; i < width_ * height_; i++) {
        data[4 * i] = (unsigned int)(data_[i].r * 255);
        data[4 * i + 1] = (unsigned int)(data_[i].g * 255);
        data[4 * i + 2] = (unsigned int)(data_[i].b * 255);
        data[4 * i + 3] = (unsigned int)(data_[i].a * 255);
    }
    unsigned int error =
        lodepng::encode("out/" + file_name, data, width_, height_);
    if (error) {
        printf("error %s: %s\n", file_name.c_str(), lodepng_error_text(error));
    }
}