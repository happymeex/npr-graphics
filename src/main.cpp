#include "Camera.hpp"
#include "Hittable.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "Scene.hpp"
#include "Shapes.hpp"
#include "Tracer.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

int main()
{
    Scene scene;
    Tracer tracer(CameraSpec{glm::vec3{0.35f, 0.6f, 0.8f},
                             glm::vec3{-0.5f, -0.5f, -1.0f},
                             glm::vec3{0.0f, 1.0f, 0.0f}, 25.0f},
                  256, 256, glm::vec3{0.0f, 0.3f, 0.3f}, 10);
    // auto sphere =
    //     std::unique_ptr<Sphere>(new Sphere(glm::vec3{0.0f, 0.0f,
    //     0.0f}, 1.0f));
    // sphere->Translate(glm::vec3{0.0f, 0.0f, 0.0f});
    // auto plane =
    //     std::unique_ptr<Plane>(new Plane(glm::vec3{0.0f, -1.0f,
    //     0.0f}, 1.0f));
    // Material red;
    // red.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
    // red.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    // red.shininess = 10.0f;
    Material silver;
    silver.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    silver.specular = glm::vec3(1.f, 1.f, 1.f);
    silver.shininess = 20.0f;
    // sphere->SetMaterial(red);
    // plane->SetMaterial(silver);

    auto bunny =
        std::unique_ptr<Mesh>(new Mesh(load_mesh_from_obj("bunny_200.obj")));

    auto bunny_obj = std::unique_ptr<Object>(new Object(std::move(bunny)));
    bunny_obj->SetMaterial(silver);

    // auto light = std::unique_ptr<PointLight>(
    //     new PointLight(glm::vec3{0.0f, -3.0f, 1.0f},
    //                    glm::vec3(0.8f, 0.5f, 0.7f), glm::vec3(0.025)));
    auto directional_light =
        std::unique_ptr<DirectionalLight>(new DirectionalLight(
            glm::vec3{0.2f, 1.f, .4f}, glm::vec3(0.7f, 0.7f, 0.7f)));

    // scene.AddObject(std::move(sphere));
    // scene.AddObject(std::move(plane));
    scene.AddObject(std::move(bunny_obj));
    // scene.AddLight(std::move(light));
    scene.AddLight(std::move(directional_light));
    tracer.Render(scene, "render_real.png", RenderStyle::Real);
    tracer.Render(scene, "render_cel.png", RenderStyle::Cel);

    return 0;
}