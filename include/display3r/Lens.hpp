#ifndef DISPLAY3R_LENS_H
#define DISPLAY3R_LENS_H

#include <string>
#include <glm/glm.hpp>

#include "display3r/Color.hpp"
#include "display3r/Config.hpp"
#include "display3r/Frame.hpp"
#include "display3r/ZBuffer.hpp"

namespace display3r {

using namespace glm;

class Camera;

class Lens : public Frame {
public:
    Lens(Camera const &camera);
    Lens(Camera const &camera, std::string const&, Config const&);

    inline bool IsValidCoordinate(ivec2 const &c) const
    {
        return ((c.y >= 0) && (c.y < m_windowHeightA) &&
                (c.x >= 0) && (c.x < m_windowWidthA));
    }

    inline float GetHFov() const { return m_hfov; }
    inline float GetWFov() const { return m_wfov; }
    inline float GetNearPlan() const { return m_nearplan; }
    inline float GetFarPlan() const { return m_farplan; }
    inline Color GetFilter() const { return m_filter; }
    inline bool Overlapping() const { return m_overlapping; }

    inline int GetWindowWidth() const { return m_windowWidthA; }
    inline int GetWindowHeight() const { return m_windowHeightA; }
    inline ivec2 GetWindowPosition() const { return m_windowPositionA; }
    inline ZBuffer *GetZBuffer() { return &m_zbuf; }

    void UpdatePosition();
    void Refresh(int width, int height);
    void Clear();

private:
    Camera const &m_camera;
    vec3 m_offset;
    float m_theta, m_phi, m_rho;
    Color m_filter;

    ivec2 m_windowPosition;
    ivec2 m_windowPositionA;

    int m_windowWidth;
    int m_windowHeight;

    int m_windowWidthA;
    int m_windowHeightA;

    bool m_overlapping;

    float m_nearplan;
    float m_farplan;
    float m_wfov;
    float m_hfov;

    ZBuffer m_zbuf;
};


};
#endif //DISPLAY3R_LENS_H
