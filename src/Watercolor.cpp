#include "Watercolor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * Constructor for the WCFX class.
 *
 * @param tex_file The path to the texture file.
 * @param width The width of the object.
 * @param height The height of the object.
 *
 * @throws None
 */
WCFX::WCFX(const std::string &tex_file, int width, int height)
    : width_(width), height_(height) {
    //
    LoadPaperTexture(tex_file);
}

/**
 * Loads a paper texture from the specified file path.
 *
 * @param tex_file The path to the paper texture file.
 *
 * @throws std::runtime_error If the file fails to open or the paper texture is
 * too small.
 */
void WCFX::LoadPaperTexture(const std::string &tex_file) {
    std::fstream file("assets/tex/" + tex_file);
    if (!file) {
        throw std::runtime_error("Failed to open file" + tex_file);
    }
    int max_width, max_height;
    file >> max_width >> max_height;
    if (max_width < width_ || max_height < height_) {
        throw std::runtime_error("Paper texture is too small");
    }
    std::string line;
    std::vector<glm::vec3> temp;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        float x, y, z;
        ss >> x >> y >> z;
        temp.push_back(glm::vec3(x, y, z));
    }
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            paper_uvh_.push_back(temp[i * width_ + j]);
        }
    }
}

glm::vec3 WCFX::apply_pigment_texture(const glm::vec3 &color, int x, int y) {
    glm::vec3 paper_uvh = GetPaperTexture(x, y);
    float H = paper_uvh.b;
    float H_r = PAPER_ROUGHNESS * H + (1.f - PAPER_ROUGHNESS) * 0.5f;
    return glm::vec3(0.f);
}

const glm::vec3 &WCFX::GetPaperTexture(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::runtime_error("GetPaperTexture: index out of range");
    }
    return paper_uvh_[y * width_ + x];
}