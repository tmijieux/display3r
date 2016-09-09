#ifndef RENDERER_H
#define RENDERER_H

#include <stack>
#include <vector>
#include <utility>
#include <glm/glm.hpp>

#include "display3r/Texture.hpp"
#include "display3r/Camera.hpp"
#include "display3r/Color.hpp"
#include "display3r/Light.hpp"
#include "display3r/Util.hpp"

using namespace glm;

namespace display3r {
class Vertex;
class Face;
class Segment;
class Pixel;
class Line;
class Triangle;

class Window;
};

namespace display3r {

class ZBuffer {
public:
    inline float &operator[](ivec2 i) { return m_buf[i.y*m_width + i.x]; }

    void Clear();
    void Resize(int width, int height);

private:
    int m_width;
    std::vector<float> m_buf;
};

class Renderer {
public:
    /* [Flags] */
    enum DrawState {
        NONE      = 0x00,
        VERTEX    = 0x01,
        WIREFRAME = 0x02,
        FACE      = 0x04,

        NORMAL    = 0x08,

    };

    Renderer(Window *window);

    // 2D objects
    void DrawPixel(ivec2 const &P, float depth);
    void DrawLine(Line const &l);
    void DrawLine(ivec2 const &A, ivec2 const &B, float dA, float dB);
    void DrawTriangle(Triangle const &t);
    void DrawTriangle(Pixel const &A, Pixel const &B, Pixel const &C);

    // 3D objects
    void DrawVertex(Vertex const &v);
    void DrawVertex(vec3 const &v);

    void DrawSegment(Segment const &s);
    void DrawSegment(vec3 const &A, vec3 const &B);

    void DrawFace(Face const &f);
    void DrawFace(vec3 const &A, vec3 const &B, vec3 const &C,
                  vec2 const &U, vec2 const &V, vec2 const &W,
                  vec3 const &nA, vec3 const &nB, vec3 const &nC);

    // Renderer State
    void Clear(); // clear renderer internals (i.e Z-buffer)
    void Resize(int width, int height);

    void BindTexture(Texture *texture);
    void BindLights(std::vector<Light> *lights);

    void SetLens(Lens const*);

    void SetDrawState(DrawState drawState);
    void SetColor(Color const &drawColor);
    void PushState();
    void PopState();

private:
    Color ComputeLight(vec3 const &pos, vec3 const &normal);
    void DrawLexel(ivec2 P, Color color);
    vec3 ProjectPoint(vec3 const &A, vec3 const& B);
    vec2 ProjectCoord(vec3 const& OA, float depth);

    void DrawTwoSubFaces(vec3 const &A, vec3 const &B, vec3 const &C,
                         float dA, float dB, float dC,
                         vec3 const &OB, vec3 const &OC,
                         vec2 const &U, vec2 const &V,  vec2 const &W,
                         vec3 const &nA, vec3 const &nB, vec3 const &nC);
    void DrawOneSubFace(vec3 const &A, vec3 const &B, vec3 const &C,
                        float dA, float dB, float dC,
                        vec3 const &OA,
                        vec2 const &U, vec2 const &V, vec2 const &W,
                        vec3 const &nA, vec3 const &nB, vec3 const &nC);

private:
    Lens const *m_lens;
    Frame m_camera;

    int m_width, m_height;
    float m_nearplan;
    float m_hfov, m_wfov;

    ZBuffer m_zbuf;
    Window *m_window;
    DrawState m_drawState;
    Color m_drawColor;
    Color m_untexturedColor;

    Texture *m_texture;
    std::vector<Light> *m_lights;

    std::stack<std::pair<DrawState,Color> > m_states;
    std::vector<float> zbuffer;
};

}; // end namespace display3r

#endif //RENDERER_H
