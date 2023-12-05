#ifndef LIGHT
#define LIGHT

#include <glm/glm.hpp>

enum class LightType {
    /** Ambient light, tints local color globally. */
    AMBIENT,
    /** Point light, tints local color based on distance, has position and
       attenuation. */
    POINT_LIGHT,
    /** Directional light, tints local color based on direction. */
    DIRECTIONAL_LIGHT
};

class Light {
  public:
    Light(LightType type) : type_(type) { color_ = glm::vec3(1.0f); }
    Light(LightType type, glm::vec3 color) : type_(type), color_(color) {}
    LightType GetType() const { return type_; }
    glm::vec3 GetColor() const { return color_; }
    virtual void GetIllumination(const glm::vec3 &hit_pos,
                                 glm::vec3 &dir_to_light,
                                 glm::vec3 &light_intensity,
                                 float &dist_to_light) = 0;

  protected:
    LightType type_;
    glm::vec3 color_;
};

class PointLight : public Light {
  public:
    PointLight(glm::vec3 position)
        : Light(LightType::POINT_LIGHT), position_(position) {
        attenuation_ = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    PointLight(glm::vec3 position, glm::vec3 color)
        : Light(LightType::POINT_LIGHT, color), position_(position) {
        attenuation_ = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation)
        : Light(LightType::POINT_LIGHT, color), position_(position),
          attenuation_(attenuation) {}

    glm::vec3 GetPosition() const { return position_; }
    glm::vec3 GetAttenuation() const { return attenuation_; }
    /**
     * Given a hit position, mutate the input references to indicate the
     * direction to, intensity of, and distance to the light from that hit.
     */
    void GetIllumination(const glm::vec3 &hit_pos, glm::vec3 &dir_to_light,
                         glm::vec3 &light_intensity,
                         float &dist_to_light) override;

  private:
    glm::vec3 position_;
    glm::vec3 attenuation_;
};

class DirectionalLight : public Light {
  public:
    DirectionalLight(glm::vec3 direction)
        : Light(LightType::DIRECTIONAL_LIGHT), direction_(direction) {}
    DirectionalLight(glm::vec3 direction, glm::vec3 color)
        : Light(LightType::DIRECTIONAL_LIGHT, color), direction_(direction) {}

    glm::vec3 GetDirection() const { return direction_; }
    void GetIllumination(const glm::vec3 &hit_pos, glm::vec3 &dir_to_light,
                         glm::vec3 &light_intensity,
                         float &dist_to_light) override;

  private:
    glm::vec3 direction_;
};

class AmbientLight : public Light {
  public:
    AmbientLight(glm::vec3 color) : Light(LightType::AMBIENT, color) {}
    void GetIllumination(const glm::vec3 &hit_pos, glm::vec3 &dir_to_light,
                         glm::vec3 &light_intensity,
                         float &dist_to_light) override;
};

#endif