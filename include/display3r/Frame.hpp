#ifndef FRAME_H
#define FRAME_H

#include <glm/glm.hpp>

namespace display3r {

struct Frame;
struct Renderer;

};

#include "display3r/Object.hpp"


using namespace glm;
namespace display3r {

struct Frame : public Object  {
    vec3 O, i, j, k;
    Frame();
    Frame(vec3 const&);

    void Rotate(vec3 const &axis, float angle);
    void Translate(vec3 const &move);
    void Reset();
protected:
    void DrawHandler(Renderer &renderer) override;
};


};

#endif // FRAME_H
