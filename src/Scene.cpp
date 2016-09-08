#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include <SDL.h>

#include "display3r/Scene.hpp"
#include "display3r/Frame.hpp"
#include "display3r/Solid.hpp"
#include "display3r/Color.hpp"
#include "display3r/Equation.hpp"

using std::string;
using display3r::Scene;
using display3r::Frame;
using display3r::Camera;
using display3r::Direction;

Scene::Scene():
    m_camera(vec3(0., -5., 0.)),
    m_origin(vec3(0.))
{
}

void Scene::AskSolid()
{
    Solid *solid;
    string objName;
    string bmpName;

    printf(".obj path: ");
    getline(std::cin, objName);
    boost::trim(objName);
    
    printf(".bmp path: ");
    getline(std::cin, bmpName);

    m_solids.push_back(Solid(objName, Texture::LoadTexture(bmpName)));
}

void Scene::AskEquation()
{
    Solid *solid;
    string eqName;
    string bmpName;

    printf(".eq path: ");

    getline(std::cin, eqName);
    
    printf(".bmp path: ");
    getline(std::cin, bmpName);
    
    Equation eq(eqName);
    /* m_solids.push_back(Solid(eq, bmpName)); */
}

void Scene::RemoveSolid()
{
    if (m_solids.size())
        m_solids.pop_back();
}

void Scene::Draw(Renderer &renderer)
{
    m_origin.Draw(renderer);
    for (auto &s : m_solids)
        s.Draw(renderer);
}

void Scene::HandleArgument(int argc, char *argv[])
{
    Solid *solid;
    switch (argc) {
    case 1:
	break;
    case 2:
        m_solids.push_back(Solid(argv[1], NULL));
	break;
    default:
	m_solids.push_back(Solid(argv[1], Texture::LoadTexture(argv[2])));
	break;
    }
}

Camera const* Scene::GetCamera() const
{
    return &this->m_camera;
}

void Scene::TranslateCamera(Direction direction)
{
    m_camera.Translate(direction);
}

void Scene::RotateCamera(Direction direction)
{
    m_camera.Rotate(direction);
}

void Scene::ToggleFrame()
{
    m_origin.ToggleVisibility();
}
