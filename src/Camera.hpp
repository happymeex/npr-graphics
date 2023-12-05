#ifndef CAMERA_SPEC
#define CAMERA_SPEC

#include "Ray.hpp"
#include <glm/glm.hpp>

struct CameraSpec {
    glm::vec3 center;
    glm::vec3 direction;
    glm::vec3 up;
    float fov;
};

class Camera {
  public:
    Camera(const CameraSpec &camera) {
        center_ = camera.center;
        direction_ = glm::normalize(camera.direction);
        up_ = glm::normalize(camera.up);
        fov_radian_ = glm::radians(camera.fov);
        horizontal_ = glm::normalize(glm::cross(direction_, up_));
    }
    Ray GetRay(float x, float y) {
        float d = 1.0f / tan(fov_radian_ / 2.0f);
        glm::vec3 new_dir = d * direction_ + x * horizontal_ + y * up_;
        new_dir = glm::normalize(new_dir);

        return Ray{center_, new_dir};
    };

  private:
    glm::vec3 center_;
    glm::vec3 direction_;
    glm::vec3 up_;
    glm::vec3 horizontal_;
    float fov_radian_;
};

#endif