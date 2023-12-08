#include "Scene.hpp"

Scene::Scene() {
    const siv::PerlinNoise::seed_type seed = 123456u;
    // Initialize the Perlin noise generator
    pigment_density_perlin_ = siv::PerlinNoise(seed);
}

float Scene::GetDensity(float x, float y) const {
    auto density = pigment_density_perlin_.normalizedOctave2D(x, y, 2);
    return 0.5f * (float)density;
}

void Scene::AddObject(std::unique_ptr<Hittable> obj) {
    children.push_back(std::move(obj));
}

void Scene::AddLight(std::unique_ptr<Light> light) {
    lights.push_back(std::move(light));
}