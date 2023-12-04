#include "util/png.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

int main() {
    int width = 256;
    int height = 256;
    std::vector<unsigned char> image(width * height * 4);
    for (int i = 0; i < width * height; i++) {
        image[4 * i] = 0;
        image[4 * i + 1] = 128;
        image[4 * i + 2] = 128;
        image[4 * i + 3] = 255;
    }
    // lodepng::encode("out/out.png", image, width, height);
    save_png("out.png", image, width, height);
    std::cout << "Hello, World!" << std::endl;
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
    return 0;
}