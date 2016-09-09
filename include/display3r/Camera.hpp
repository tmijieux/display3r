#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace display3r { class Camera; }

#include "display3r/Frame.hpp"
#include "display3r/Lens.hpp"
#include "display3r/Config.hpp"

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
    Camera();
    Camera(std::string const &name, Config const &conf);
    
    void Rotate(Direction);
    void Translate(Direction);
    std::vector<Lens> const &GetLenses();

protected:
    float theta, phi, rho;
    float vspeed;
    float rspeed;
private:
    std::vector<Lens> m_lenses;
};

};
#endif //CAMERA_H
