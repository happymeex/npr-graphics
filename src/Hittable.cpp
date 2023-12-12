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
    float x = density_scale_ * position.x;
    float y = density_scale_ * position.y;
    float z = density_scale_ * position.z;
    float density =
        (float)(pigment_density_perlin_.normalizedOctave3D(x, y, z, 2));
    return density_strength_ * density;
}