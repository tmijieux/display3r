#include <math.h>
#include "display3r/Lens.hpp"
#include "display3r/Camera.hpp"

#define DEGREE_TO_RADIAN(x) ((x) * M_PI / 180.)
using display3r::Lens;
using display3r::Color;

Lens::Lens(Camera const &camera):
    m_camera(camera),
    m_offset(0.f, 0.f, 0.f), m_overlapping(false),
    m_theta(0), m_phi(0.), m_rho(0.),
    m_filter(255, 255, 255),
    m_windowPosition(ivec2(0,0)),
    m_windowWidth(8), m_windowHeight(8),
    m_nearplan(1.), m_farplan(20.),
    m_wfov(DEGREE_TO_RADIAN(90.)),
    m_hfov(M_PI / 2.)
{
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

float Lens::GetHFov() const { return m_hfov; }
float Lens::GetWFov() const { return m_wfov; }
float Lens::GetNearPlan() const { return m_nearplan; }
float Lens::GetFarPlan() const { return m_farplan; }
Color Lens::GetFilter() const { return m_filter; }
bool Lens::Overlapping() const { return m_overlapping; }

int Lens::GetWindowWidth() const { return m_windowWidthA; }
int Lens::GetWindowHeight() const { return m_windowHeightA; }
ivec2 Lens::GetWindowPosition() const { return m_windowPositionA; }
