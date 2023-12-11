#ifndef WATERCOLOR
#define WATERCOLOR

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Paper {
  public:
    Paper(const std::string &tex_path, int width, int height,
          float light_angle);
    const float PAPER_ROUGHNESS = 0.5f;

    glm::vec3 apply_pigment_turbulence(const glm::vec3 &color, float density);
    glm::vec3 apply_pigment_texture(const glm::vec3 &color, int x, int y);
    glm::vec3 GetPaperIllumination(int x, int y);

  private:
    int width_;
    int height_;
    std::vector<glm::vec3> paper_uvh_;
    glm::vec3 light_dir_vec_;

    void LoadPaperTexture(const std::string &tex_file);
    const glm::vec3 &GetPaperTexture(int x, int y) const;
};

#endif