#ifndef DISPLAY3R_FRAME_H
#define DISPLAY3R_FRAME_H

#include <glm/glm.hpp>

namespace display3r { struct Frame; };

#include "display3r/Drawable.hpp"

namespace display3r {

class Renderer;
using glm::ivec2;
using glm::vec3;
using glm::vec2;

struct Frame : public Drawable  {
    vec3 O;
    vec3 i, j, k;

    Frame();
    Frame(vec3 const&);

    void Rotate(vec3 const &axis, float angle);
    void Translate(vec3 const &move);
    void Reset();

protected:
    virtual void OnMovement() {}
	virtual void DrawHandler(Renderer &renderer) override;
};


};

#endif // DISPLAY3R_FRAME_H
