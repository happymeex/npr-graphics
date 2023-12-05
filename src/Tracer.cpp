#include "Tracer.hpp"
#include "Image.hpp"

void Tracer::Render(const Scene &scene, const std::string &out_file_name) {
    //
    Image image(width_, height_);
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            glm::vec4 color = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
            image.SetPixel(x, y, color);
        }
    }
    image.SavePNG(out_file_name);
}