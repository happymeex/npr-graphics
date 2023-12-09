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

void ball() {
    Scene scene;
    Tracer tracer(CameraSpec{glm::vec3{0.f, 0.f, 6.f},
                             glm::vec3{0.f, 0.f, -1.0f},
                             glm::vec3{0.0f, 1.0f, 0.0f}, 45.0f},
                  256, 256, glm::vec3{0.0f, 0.0f, 0.0f}, 10);
    Material orange;
    orange.diffuse = glm::vec3(1.0f, 0.5f, 0.0f);
    orange.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    orange.shininess = 10.0f;
    Material white;
    white.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    white.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    white.shininess = 10.0f;

    auto sphere =
        std::unique_ptr<Sphere>(new Sphere(glm::vec3{0.f, 0.f, 0.f}, 1.0f));
    sphere->SetMaterial(orange);
    auto plane =
        std::unique_ptr<Plane>(new Plane(glm::vec3{0.f, 1.f, 0.f}, 1.f));
    plane->SetMaterial(white);

    auto point_light = std::unique_ptr<Light>(new PointLight(
        glm::vec3{2.f, 5.f, 8.f}, glm::vec3{8.f, 5.f, 7.f}, glm::vec3(0.025f)));
    auto ambient_light =
        std::unique_ptr<Light>(new AmbientLight(glm::vec3(0.5f)));

    scene.AddObject(std::move(sphere));
    scene.AddObject(std::move(plane));
    // scene.AddLight(std::move(directional_light));
    scene.AddLight(std::move(point_light));
    scene.AddLight(std::move(ambient_light));

    tracer.Render(scene, "ball_real.png", RenderStyle::Real);
    tracer.Render(scene, "ball_cel.png", RenderStyle::Cel);
    tracer.Render(scene, "ball_watercolor.png", RenderStyle::Watercolor);
}

void bunny() {
    Scene scene;
    Tracer tracer(CameraSpec{glm::vec3{0.35f, 0.6f, 0.8f},
                             glm::vec3{-0.5f, -0.5f, -1.0f},
                             glm::vec3{0.0f, 1.0f, 0.0f}, 25.0f},
                  256, 256, glm::vec3{0.1f, 0.7f, 0.2f}, 10);

    Material silver;
    silver.diffuse = glm::vec3(0.79f, 0.66f, 0.44f);
    silver.specular = glm::vec3(1.f, 1.f, 1.f);
    silver.shininess = 20.0f;

    auto bunny =
        std::unique_ptr<Mesh>(new Mesh(load_mesh_from_obj("bunny_200.obj")));

    auto bunny_obj = std::unique_ptr<Object>(new Object(std::move(bunny)));
    bunny_obj->SetMaterial(silver);

    auto directional_light =
        std::unique_ptr<DirectionalLight>(new DirectionalLight(
            glm::vec3{0.2f, -1.f, -.4f}, glm::vec3(0.7f, 0.7f, 0.7f)));
    auto directional_light2 =
        std::unique_ptr<DirectionalLight>(new DirectionalLight(
            glm::vec3{-7.f, -5.f, -5.f}, glm::vec3(0.3f, 0.3f, 0.3f)));
    auto ambient_light =
        std::unique_ptr<AmbientLight>(new AmbientLight(glm::vec3(0.1f)));

    scene.AddObject(std::move(bunny_obj));
    scene.AddLight(std::move(directional_light));
    scene.AddLight(std::move(directional_light2));
    scene.AddLight(std::move(ambient_light));
    tracer.Render(scene, "bunny_real.png", RenderStyle::Real);
    tracer.Render(scene, "bunny_cel.png", RenderStyle::Cel);
    tracer.Render(scene, "bunny_watercolor.png", RenderStyle::Watercolor);
}