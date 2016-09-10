#include <iostream>
#include "display3r/Camera.hpp"
#include "display3r/Util.hpp"
#include "display3r/Lens.hpp"

using display3r::Camera;
using display3r::Direction;
using display3r::Lens;
using namespace std;

Camera::Camera():
    Frame(vec3(0.f, -5.f, 0.f)),
    vspeed(0.1), rspeed(0.1)
{
    m_lenses.push_back(Lens(*this));
}

Camera::Camera(std::string const &name, Config const &conf)
{
    std::string id = "camera." + name + ".";
    vspeed = conf[id+"translationSpeed"].as<float>();
    rspeed = conf[id+"rotationSpeed"].as<float>();
    O = ParseVec3(conf[id+"position"].as<string>());
    theta = conf[id+"theta"].as<float>();
    phi = conf[id+"phi"].as<float>();
    rho = conf[id+"rho"].as<float>();

    vector<string> lenses;
    lenses = conf[id+"lens"].as<vector<string> >();

    cout << "Camera '" << name << "':" << endl;
    for (auto &l : lenses) {
        try {
            cout << "Loading 'lens." << l << "' ... ";
            m_lenses.push_back(Lens(*this, l, conf));
            cout << "Success." << endl;
        } catch (std::exception &e) {
            cout << " FAILED!" << endl;
        }
    }
}

void Camera::Rotate(Direction direction)
{
    switch (direction) {
    case LEFT:  Frame::Rotate(k, -rspeed);    break;
    case RIGHT: Frame::Rotate(k, rspeed);     break;
    case UP:    Frame::Rotate(i, rspeed);     break;
    case DOWN:  Frame::Rotate(i, -rspeed);    break;
    default:
        break;
    }
}

void Camera::Translate(Direction direction)
{
    switch (direction) {
    case BACKWARD: Frame::Translate(-j * vspeed);     break;
    case FORWARD:  Frame::Translate(j * vspeed);      break;
    case LEFT:     Frame::Translate(-i * vspeed);     break;
    case RIGHT:    Frame::Translate(i * vspeed);      break;
    case DOWN:     Frame::Translate(-k * vspeed);     break;
    case UP:       Frame::Translate(k *vspeed);       break;

    }
}

std::vector<Lens> &Camera::GetLenses()
{
    return m_lenses;
}

void Camera::OnMovement()
{
    for (auto &l : m_lenses)
        l.UpdatePosition();
}
