#include "Light.hpp"

void PointLight::GetIllumination(const glm::vec3 &hit_pos,
                                 glm::vec3 &dir_to_light,
                                 glm::vec3 &light_intensity,
                                 float &dist_to_light) {
    //
    glm::vec3 attenuation = GetAttenuation();

    glm::vec3 light_pos = GetPosition();
    dist_to_light = glm::length(light_pos - hit_pos);
    dir_to_light = glm::normalize(light_pos - hit_pos);
    light_intensity =
        GetColor() / (attenuation * dist_to_light * dist_to_light);
}

void AmbientLight::GetIllumination(const glm::vec3 &hit_pos,
                                   glm::vec3 &dir_to_light,
                                   glm::vec3 &light_intensity,
                                   float &dist_to_light) {
    light_intensity = GetColor();
}

void DirectionalLight::GetIllumination(const glm::vec3 &hit_pos,
                                       glm::vec3 &dir_to_light,
                                       glm::vec3 &light_intensity,
                                       float &dist_to_light) {
    dir_to_light = -GetDirection();
    light_intensity = GetColor();
    dist_to_light = std::numeric_limits<float>::max();
}