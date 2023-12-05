#ifndef HITRECORD
#define HITRECORD

#include <glm/glm.hpp>

struct HitRecord {
    HitRecord() { time = std::numeric_limits<float>::max(); }

    float time;
    glm::vec3 normal;
};

#endif