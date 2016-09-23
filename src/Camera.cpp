#include <iostream>
#include "display3r/Camera.hpp"
#include "display3r/Util.hpp"
#include "display3r/Lens.hpp"
#include "display3r/Config.hpp"

using namespace display3r;
using namespace std;

Camera::Camera():
    Frame(vec3(0.f, -5.f, 0.f)),
    vspeed(0.1f), rspeed(0.1f)
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
    Frame::Rotate(i, theta);
    Frame::Rotate(j, phi);
    Frame::Rotate(k, rho);

    cout << "Camera '" << name << "':" << endl;
    for (auto &l : lenses) {
        try {
            cout << "Loading 'lens." << l << "' ... ";
            m_lenses.push_back(Lens(*this, l, conf));
            cout << "Success." << endl;
        } catch (std::exception &e) {
			(void) e;
            cout << " FAILED!" << endl;
        }
    }
}

void Camera::Rotate(Direction direction)
{
    static vec3 up(0.f, 0.f, 1.f);

    switch (direction) {
    case LEFT:  Frame::Rotate(up, rspeed);    break;
    case RIGHT: Frame::Rotate(up, -rspeed);   break;
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
