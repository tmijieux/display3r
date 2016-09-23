#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <vector>

#include "display3r/Lens.hpp"
#include "display3r/Camera.hpp"
#include "display3r/Util.hpp"
#include "display3r/Config.hpp"

#define MAXWINDOWS 8

using namespace display3r;
using namespace std;

Lens::Lens(Camera const &camera):
    m_camera(camera),
    m_offset(0.f, 0.f, 0.f),
    m_theta(0), m_phi(0.), m_rho(0.),
    m_filter(255, 255, 255),
    m_windowPosition(ivec2(0,0)),
    m_windowPositionA(ivec2(0,0)),
    m_windowWidth(8), m_windowHeight(8),
    m_windowWidthA(0), m_windowHeightA(0),
    m_overlapping(false),
    m_nearplan(1.), m_farplan(20.),
    m_wfov(DEGREE_TO_RADIAN(90.)),
    m_hfov(0.0f)
{
    UpdatePosition();
}

Lens::Lens(Camera const &camera, string const &name, Config const &conf):
    m_camera(camera),
    m_windowWidthA(0),
    m_windowHeightA(0),
    m_hfov(0.)
{
    string id = "lens." + name + ".";
    m_offset = ParseVec3(conf[id+"offset"].as<string>());
    m_theta = conf[id+"theta"].as<float>();
    m_phi = conf[id+"phi"].as<float>();
    m_rho = conf[id+"rho"].as<float>();
    m_filter = Color::FromString(conf[id+"filter"].as<string>());
    int x, y;
    x = conf[id+"screenPositionWidth"].as<int>();
    y = conf[id+"screenPositionHeight"].as<int>();
    m_windowPosition = ivec2(x, y);
    m_windowWidth = conf[id+"screenWidth"].as<int>();
    m_windowHeight = conf[id+"screenHeight"].as<int>();
    m_overlapping = conf[id+"overlapping"].as<bool>();
    m_nearplan = conf[id+"nearplan"].as<float>();
    m_farplan = conf[id+"farplan"].as<float>();
    m_wfov = DEGREE_TO_RADIAN(conf[id+"wfov"].as<float>());

    UpdatePosition();
}

void Lens::Refresh(int width, int height)
{
    m_windowPositionA = m_windowPosition * ivec2(width, height) / MAXWINDOWS;
    m_windowWidthA = m_windowWidth * width / MAXWINDOWS;
    m_windowHeightA = m_windowHeight * height / MAXWINDOWS;
    m_hfov = 2*atan(tan(m_wfov/2.))*((float)m_windowWidthA/m_windowHeightA);
    m_zbuf.Resize(m_windowWidthA, m_windowHeightA);
}

void Lens::Clear()
{
    m_zbuf.Clear();
}

void Lens::UpdatePosition()
{
    i = m_camera.i;
    j = m_camera.j;
    k = m_camera.k;

    this->Rotate(m_camera.k, m_theta);
    this->Rotate(m_camera.i, m_phi);
    this->Rotate(m_camera.k, m_rho);

    O = m_camera.O + i * m_offset.x + j * m_offset.y + k * m_offset.z;
}

