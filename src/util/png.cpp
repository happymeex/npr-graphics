#include "png.hpp"

void save_png(const std::string &path, const std::vector<unsigned char> &image,
              int width, int height) {
    lodepng::encode("out/" + path, image, width, height);
}