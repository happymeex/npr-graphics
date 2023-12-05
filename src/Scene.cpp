#include "Scene.hpp"

Scene::Scene() {}

void Scene::AddObject(std::unique_ptr<Hittable> obj) {
    children.push_back(std::move(obj));
}