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
                  256, 256, glm::vec3{0.6f, 0.7f, 1.0f}, 10);
    Material orange;
    orange.diffuse = glm::vec3(1.0f, 0.5f, 0.0f);
    orange.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    orange.shininess = 10.0f;
    Material white;
    white.diffuse = glm::vec3(.6f, .6f, .6f);
    white.specular = glm::vec3(0.2f, 0.2f, 0.2f);
    white.shininess = 10.0f;

    auto sphere =
        std::unique_ptr<Sphere>(new Sphere(glm::vec3{0.f, 0.f, 0.f}, 1.0f));
    sphere->SetMaterial(orange);
    auto plane =
        std::unique_ptr<Plane>(new Plane(glm::vec3{0.f, 1.f, 0.f}, 1.f));
    plane->SetMaterial(white);

    auto point_light = std::unique_ptr<Light>(new PointLight(
        glm::vec3{2.f, 4.f, 3.f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec3(0.025f)));
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

    Material brown;
    brown.diffuse = glm::vec3(0.79f, 0.66f, 0.44f);
    brown.specular = glm::vec3(1.f, 1.f, 1.f);
    brown.shininess = 20.0f;

    auto bunny = std::make_unique<Mesh>(load_mesh_from_obj("bunny_200.obj"));

    auto bunny_obj = std::make_unique<Object>(std::move(bunny));
    bunny_obj->SetMaterial(brown);

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

void rustacean() {
    Scene scene;
    Tracer tracer(CameraSpec{glm::vec3{-2.f, 1.f, 1.8f},
                             glm::vec3{1.0f, -0.5f, -1.0f},
                             glm::vec3{0.0f, 1.0f, 0.0f}, 45.0f},
                  512, 512, glm::vec3{0.1f, 0.1f, 0.2f}, 10);
    Material orange;
    orange.diffuse = glm::vec3(1.0f, 0.5f, 0.0f);
    orange.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    orange.shininess = 10.0f;
    Material white;
    white.diffuse = glm::vec3(.6f, .6f, .6f);
    white.specular = glm::vec3(0.2f, 0.2f, 0.2f);
    white.shininess = 10.0f;

    auto plane = std::make_unique<Plane>(glm::vec3{0.f, 1.f, 0.f}, 0.12f);
    plane->SetMaterial(white);
    plane->SetDensitySeed(7);
    plane->SetDensityStrength(0.5f);

    auto rustacean =
        std::make_unique<Mesh>(load_mesh_from_obj("rustacean.obj"));
    auto rustacean_obj = std::make_unique<Object>(std::move(rustacean));
    rustacean_obj->SetMaterial(orange);

    auto point_light = std::unique_ptr<Light>(new PointLight(
        glm::vec3{2.f, 4.f, 5.f}, glm::vec3{1.f, 1.f, 1.f}, glm::vec3(0.025f)));
    auto ambient_light =
        std::unique_ptr<Light>(new AmbientLight(glm::vec3(0.5f)));

    scene.AddObject(std::move(rustacean_obj));
    scene.AddObject(std::move(plane));
    scene.AddLight(std::move(point_light));
    scene.AddLight(std::move(ambient_light));

    tracer.Render(scene, "rustacean_real.png", RenderStyle::Real);
    tracer.Render(scene, "rustacean_cel.png", RenderStyle::Cel);
    tracer.Render(scene, "rustacean_watercolor.png", RenderStyle::Watercolor);
}

void teapot() {
    Scene scene;
    Tracer tracer(CameraSpec{glm::vec3{0.f, 5.2f, 8.f},
                             glm::vec3{0.0f, -0.5f, -1.0f},
                             glm::vec3{0.0f, 1.0f, 0.0f}, 45.0f},
                  512, 512, glm::vec3{0.8f, 0.6f, 0.2f}, 10);
    Material silver;
    silver.diffuse = glm::vec3(.7f, 0.8f, 0.9f);
    silver.specular = glm::vec3(0.8f, 0.4f, 0.2f);
    silver.shininess = 5.0f;
    Material wood;
    wood.diffuse = glm::vec3(0.3f, 0.2f, 0.1f);
    wood.specular = glm::vec3(0.8f, 0.4f, 0.2f);
    wood.shininess = 15.0f;
    Material pink;
    pink.diffuse = glm::vec3(.8f, 0.3f, 0.5f);
    pink.specular = glm::vec3(0.9f, 0.9f, 0.7f);
    pink.shininess = 20.0f;

    auto sphere = std::make_unique<Sphere>(glm::vec3(0.f), 3.0f);
    sphere->Translate(glm::vec3{-1.f, 3.12f, -4.f});
    sphere->SetMaterial(pink);
    sphere->SetDensityStrength(0.5f);
    sphere->SetDensityScale(0.4f);

    auto small_sphere =
        std::make_unique<Sphere>(glm::vec3(1.f, 1.12f, 2.f), .5f);
    small_sphere->Translate(glm::vec3{0.3f, 0.f, 0.5f});
    small_sphere->SetMaterial(pink);
    small_sphere->SetDensityStrength(0.5f);

    auto plane = std::make_unique<Plane>(glm::vec3{0.f, 1.f, 0.f}, -.12f);
    plane->SetMaterial(wood);
    plane->SetDensityStrength(0.5f);

    auto teapot = std::make_unique<Mesh>(load_mesh_from_obj("teapot.obj"));
    auto teapot_obj = std::unique_ptr<Object>(new Object(std::move(teapot)));
    teapot_obj->SetMaterial(silver);
    teapot_obj->SetDensityStrength(1.5f);
    teapot_obj->SetDensitySeed(9);

    auto point_light = std::unique_ptr<Light>(new PointLight(
        glm::vec3{3.f, 4.f, 3.f}, glm::vec3{.8f, .6f, .6f}, glm::vec3(0.1f)));
    auto ambient_light =
        std::unique_ptr<Light>(new AmbientLight(glm::vec3(0.5f)));

    scene.AddObject(std::move(teapot_obj));
    scene.AddObject(std::move(plane));
    scene.AddObject(std::move(sphere));
    scene.AddObject(std::move(small_sphere));
    scene.AddLight(std::move(point_light));
    scene.AddLight(std::move(ambient_light));

    tracer.Render(scene, "teapot_real.png", RenderStyle::Real);
    tracer.Render(scene, "teapot_cel.png", RenderStyle::Cel);
    tracer.Render(scene, "teapot_watercolor.png", RenderStyle::Watercolor);
}