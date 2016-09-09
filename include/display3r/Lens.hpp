#ifndef LENS_H
#define LENS_H

#include <glm/glm.hpp>
#include "display3r/Color.hpp"
#include "display3r/Config.hpp"
#include "display3r/Frame.hpp"

namespace display3r {

using namespace glm;
class Camera;

class Lens : public Frame {
public:
    Lens(Camera const &camera);
    Lens(Camera const &camera, std::string const&, Config const&);

    int GetWindowWidth() const;
    int GetWindowHeight() const;
    ivec2 GetWindowPosition() const;
    bool Overlapping() const;

    float GetHFov() const;
    float GetWFov() const;
    float GetNearPlan() const;
    float GetFarPlan() const;

    Color GetFilter() const;

    void UpdatePosition();

private:
    Camera const &m_camera;
    vec3 m_offset;
    float m_theta, m_phi, m_rho;
    Color m_filter;

    ivec2 m_windowPosition;
    ivec2 m_windowPositionA;
    int m_windowWidthA;
    int m_windowWidth;
    int m_windowHeightA;
    int m_windowHeight;
    bool m_overlapping;

    float m_nearplan;
    float m_farplan;
    float m_wfov;
    float m_hfov;
};


};
#endif //LENS_H
