#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include <SDL.h>

#include "display3r/Scene.hpp"
#include "display3r/Config.hpp"
#include "display3r/Frame.hpp"
#include "display3r/Solid.hpp"
#include "display3r/Color.hpp"
#include "display3r/Equation.hpp"
#include "display3r/Renderer.hpp"

using std::string;
using std::cout;
using std::endl;
using display3r::Scene;
using display3r::Frame;
using display3r::Camera;
using display3r::Direction;

Scene::Scene(Config const &conf):
    m_camera(conf.SceneCamera, conf)
{
    vector<string> lights;
    try {
        lights = conf["scene.light"].as<vector<string> >();
    } catch (std::exception &e) {
        cout << "Exception scene: "  << e.what() << endl;
    }
    for (auto &l : lights) {
        try {
            cout << "Loading 'light."<<l<<"' ... ";
            m_lights.push_back(Light(l, conf));
            cout << "Success." << endl;
        } catch (std::exception &e) {
            cout << "ERROR!" << endl;
        }
    }
}

void Scene::AskSolid()
{
    string objName;
    string bmpName;

    printf(".obj path: ");
    getline(std::cin, objName);
    boost::trim(objName);

    printf(".bmp path: ");
    getline(std::cin, bmpName);

    try {
        Texture *t = Texture::LoadTexture(bmpName);
        m_solids.push_back(Solid(objName, t));
    } catch (std::exception &e) {
        cout << e.what() << endl;
    }
}

void Scene::Clear()
{
    for (auto &l : m_camera.GetLenses())
        l.Clear();
}

void Scene::NotifyResize(int width, int height)
{
    for (auto &l : m_camera.GetLenses())
        l.Refresh(width, height);
}

void Scene::AskEquation()
{
    string eqName;
    string bmpName;

    printf(".eq path: ");

    getline(std::cin, eqName);

    printf(".bmp path: ");
    getline(std::cin, bmpName);

    try {
        Equation eq(eqName);
        Texture *t = Texture::LoadTexture(bmpName);
        m_solids.push_back(Solid(eq, t));
    } catch (std::exception &e) {
        cout << e.what() << endl;
    }
}

void Scene::RemoveSolid()
{
    if (m_solids.size())
        m_solids.pop_back();
}

void Scene::Draw(Renderer &renderer)
{
    renderer.BindLights(&m_lights);

    for (auto &lens : m_camera.GetLenses())
    {
        renderer.SetLens(&lens);
        m_origin.Draw(renderer);
        for (auto &s : m_solids)
            s.Draw(renderer);
    }
}

void Scene::HandleArgument(int argc, char *argv[])
{
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
