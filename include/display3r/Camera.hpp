#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace display3r { class Camera; }

#include "display3r/Frame.hpp"

using namespace glm;

namespace display3r {

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
};

class Camera : public Frame {
public:
    Camera(vec3 const &pos);

    void Rotate(Direction);
    void Translate(Direction);
    
    
    float nearplan;
    float farplan;
    float wfov;
    float hfov;

    float vspeed;
    float rspeed;
};

};
#endif //CAMERA_H
