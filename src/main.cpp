#include "Camera.hpp"
#include "Hittable.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "Shapes.hpp"
#include "Tracer.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

int main() {
    Scene scene;
    Tracer tracer(CameraSpec{glm::vec3{0.0f, 0.0f, 10.0f},
                             glm::vec3{0.0f, 0.0f, -1.0f},
                             glm::vec3{0.0f, 1.0f, 0.0f}, 30.0f},
                  256, 256, glm::vec3{0.0f, 0.1f, 0.1f}, 10);
    auto sphere =
        std::unique_ptr<Sphere>(new Sphere(glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f));
    sphere->Translate(glm::vec3{0.0f, 0.0f, 0.0f});
    Material material;
    material.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 10.0f;
    sphere->SetMaterial(material);

    auto light = std::unique_ptr<PointLight>(
        new PointLight(glm::vec3{0.0f, -5.0f, 8.0f},
                       glm::vec3(0.8f, 0.5f, 0.7f), glm::vec3(0.025)));

    scene.AddObject(std::move(sphere));
    scene.AddLight(std::move(light));
    tracer.Render(scene, "render.png");

    std::cout << "Hello, World!" << std::endl;

    NPR::Mesh m = NPR::load_mesh_from_obj("c3.obj");
    std::cout << "number of vertices: " << m.vertices->size() << std::endl;
    std::cout << "number of faces: " << m.indices->size() / 3 << std::endl;

    return 0;
}