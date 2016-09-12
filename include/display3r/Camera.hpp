#ifndef DISPLAY3R_CAMERA_H
#define DISPLAY3R_CAMERA_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace display3r { class Camera; }

#include "display3r/Frame.hpp"
#include "display3r/Lens.hpp"

using namespace glm;

namespace display3r {
class Config;

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
    std::vector<Lens> &GetLenses();

protected:
    void OnMovement() override;
    void DrawHandler(Renderer&) override {/*i'm invisible!*/}

    float theta, phi, rho;
    float vspeed;
    float rspeed;

private:
    std::vector<Lens> m_lenses;
};

};
#endif //DISPLAY3R_CAMERA_H
