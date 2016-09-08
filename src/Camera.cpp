#include "display3r/Camera.hpp"

using display3r::Camera;
using display3r::Direction;

Camera::Camera(vec3 const &pos):
    Frame(pos),
    nearplan(0.1), farplan(100.),
    wfov(30.), hfov(30.),
    vspeed(1.0), rspeed(0.1)
{
}

void Camera::Rotate(Direction direction)
{
    switch (direction) {
    case LEFT:  Frame::Rotate(k, -rspeed);    break;
    case RIGHT: Frame::Rotate(k, rspeed);     break;
    case UP:    Frame::Rotate(i, rspeed);     break;
    case DOWN:  Frame::Rotate(i, -rspeed);    break;
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
