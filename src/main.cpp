#include "util/Mesh.hpp"
#include "util/png.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

int main() {
    int width = 256;
    int height = 256;
    std::vector<unsigned char> image(width * height * 4);
    for (int i = 0; i < width * height; i++) {
        image[4 * i] = 128;
        image[4 * i + 1] = 128;
        image[4 * i + 2] = 0;
        image[4 * i + 3] = 255;
    }
    // lodepng::encode("out/out.png", image, width, height);
    save_png("out2.png", image, width, height);
    std::cout << "Hello, World!" << std::endl;
    glm::vec3 v(1.0f, 2.0f, 3.0f);
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;

    NPR::Mesh m = NPR::load_mesh_from_obj("c3.obj");
    std::cout << "number of vertices: " << m.vertices->size() << std::endl;
    std::cout << "number of faces: " << m.indices->size() / 3 << std::endl;

    return 0;
}