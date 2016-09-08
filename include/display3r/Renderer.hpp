#ifndef RENDERER_H
#define RENDERER_H

#include <stack>
#include <vector>
#include <utility>
#include <glm/glm.hpp>

#include "display3r/Texture.hpp"
#include "display3r/Camera.hpp"
#include "display3r/Color.hpp"

using namespace glm;

namespace display3r {
class Vertex;
class Face;
class Segment;
class Window;
};

namespace display3r {

struct Pixel {
    ivec2 A;
    float depth;
    Pixel(const ivec2& a, float Depth): A(a), depth(Depth) {}
};

struct Line {
    ivec2 A, B;
    float dA, dB;
};

struct Triangle {
    ivec2 A, B, C; // 2D coords
    float dA, dB, dC; // depth
    vec2 U, V, W; // tex coords
    vec3 nA, nB, nC; // normals
};

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
    
    Renderer(Camera const *camera, Window *window);
    
    // 2D objects
    void DrawPixel(Pixel const &p);
    void DrawLine(Line const &l);
    void DrawLine(ivec2 const &A, ivec2 const &B, float dA, float dB);
    void DrawTriangle(Triangle const &t);
    void DrawTriangle(ivec2 const &A, ivec2 const &B, ivec2 const &C,
                      float dA, float dB, float dC,
                      vec2 const &U, vec2 const &V, vec2 const &W,
                      vec3 const &nA, vec3 const &nB, vec3 const &nC);
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
    void SetDrawState(DrawState drawState);
    void SetColor(Color const &drawColor);
    void PushState();
    void PopState();

private:
    void DrawPixelInternal(Pixel const &p, Color c);

    vec3 ProjectPoint(vec3 const &A, vec3 const& B);
    vec2 ProjectCoord(vec3 const& OA, float depth);

private:
    Camera const *m_camera;
    Window *m_window;
    DrawState m_drawState;
    Color m_drawColor;
    ZBuffer m_zbuf;

    vec3 m_light;

    Texture *m_texture;

    std::stack<std::pair<DrawState,Color> > m_states;
    std::vector<float> zbuffer;
};

}; // end namespace display3r

#endif //RENDERER_H
