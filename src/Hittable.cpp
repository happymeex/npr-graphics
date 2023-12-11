#include "Hittable.hpp"

Hittable::Hittable() {
    translation_ = glm::vec3(0.0f, 0.0f, 0.0f);
    // random seed
    siv::PerlinNoise::seed_type seed = 14156u;
    // Initialize the Perlin noise generator
    pigment_density_perlin_ = siv::PerlinNoise(seed);
}

float Hittable::GetDensity(const glm::vec2 &img_pos,
                           const glm::vec3 &position) const {
    float density = (float)(pigment_density_perlin_.normalizedOctave3D(
        position.x, position.y, position.z, 2));
    // TODO: Better way to do this
    return density;
}