#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "display3r/Frame.hpp"
#include "display3r/Color.hpp"
#include "display3r/Object3D.hpp"
#include "display3r/Renderer.hpp"

//#include "display3r/project.hpp"

using namespace std;
using display3r::Frame;
using display3r::Vertex;

Frame::Frame():
    O(0.), i(1., 0., 0.), j(0., 1., 0.), k(0., 0., 1.)
{
}

Frame::Frame(vec3 const &pos):
    O(pos), i(1., 0., 0.), j(0., 1., 0.), k(0., 0., 1.)
{
}

void Frame::Reset()
{
    *this = Frame();
    OnMovement();
}

void Frame::DrawHandler(Renderer &renderer)
{
    Color color;

    renderer.PushState();
    renderer.SetColor(Color::RED);
    renderer.DrawSegment(O, i);

    renderer.SetColor(Color::GREEN);
    renderer.DrawSegment(O, j);

    renderer.SetColor(Color::BLUE);
    renderer.DrawSegment(O, k);
    renderer.PopState();
}

void Frame::Rotate(vec3 const &axis, float angle)
{
    mat4 R = glm::rotate(angle, axis);
    i = normalize(vec3( R * vec4(i, 1.0) ));
    j = normalize(vec3( R * vec4(j, 1.0) ));
    k = normalize(vec3( R * vec4(k, 1.0) ));
    OnMovement();
}

void Frame::Translate(vec3 const &move)
{
    O += move;
    OnMovement();
}
