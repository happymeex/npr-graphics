#include "Paper.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/constants.hpp>

/**
 * Constructor for the Paper class.
 *
 * @param tex_file The path to the texture file.
 * @param width The width of the object.
 * @param height The height of the object.
 *
 * @throws None
 */
Paper::Paper(const std::string &tex_file, int width, int height,
             float light_angle)
    : width_(width), height_(height) {
    LoadPaperTexture(tex_file);
    float light_angle_radians = light_angle * glm::pi<float>() / 180.f;
    light_dir_vec_ =
        glm::normalize(glm::vec3(cos(light_angle), sin(light_angle), -1.0f));
}

/**
 * Loads a paper texture from the specified file path.
 *
 * @param tex_file The path to the paper texture file.
 *
 * @throws std::runtime_error If the file fails to open or the paper texture is
 * too small.
 */
void Paper::LoadPaperTexture(const std::string &tex_file) {
    std::fstream file("assets/tex/" + tex_file);
    if (!file) {
        throw std::runtime_error("Failed to open file " + tex_file);
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
        x /= 255.f;
        y /= 255.f;
        z /= 255.f;
        temp.push_back(glm::vec3(x, y, z));
    }
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            paper_uvh_.push_back(temp[i * max_width + j]);
        }
    }
}

glm::vec3 Paper::GetPaperIllumination(int x, int y) {
    glm::vec3 paper_tex = GetPaperTexture(x, y) - glm::vec3(0.5f);
    glm::vec3 normal =
        glm::normalize(glm::vec3(-paper_tex.x, -paper_tex.y, 1.f));
    float alpha = glm::dot(normal, light_dir_vec_); // between -1 and 1
    alpha = 1.f - 0.25f * glm::pow(alpha, 2.f);
    return glm::vec3(alpha);
}

glm::vec3 Paper::apply_pigment_texture(const glm::vec3 &color, int x, int y) {
    glm::vec3 paper_uvh = GetPaperTexture(x, y);
    float H = paper_uvh.b;
    float H_r = PAPER_ROUGHNESS * H + (1.f - PAPER_ROUGHNESS) * 0.5f;
    return glm::vec3(0.f);
}

const glm::vec3 &Paper::GetPaperTexture(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::runtime_error("GetPaperTexture: index out of range");
    }
    return paper_uvh_[y * width_ + x];
}