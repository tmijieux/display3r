#ifndef FRAME_H
#define FRAME_H

#include <glm/glm.hpp>

namespace display3r {

struct Frame;
struct Renderer;

};

#include "display3r/Drawable.hpp"


using namespace glm;
namespace display3r {

struct Frame : public Drawable  {

    union {
        vec3 O;
        vec3 position;
    };
    vec3 i, j, k;

    Frame();
    Frame(vec3 const&);

    void Rotate(vec3 const &axis, float angle);
    void Translate(vec3 const &move);
    void Reset();


protected:
    virtual void OnMovement() {}
    void DrawHandler(Renderer &renderer) override;
};


};

#endif // FRAME_H
