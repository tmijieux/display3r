#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>
#include <memory>

#include "display3r/Frame.hpp"
#include "display3r/Solid.hpp"
#include "display3r/Camera.hpp"
#include "display3r/Light.hpp"

namespace display3r {

class Config;
class Renderer;

// TODO move this
enum scene_defaults {
    DEFAULT_WIDTH_FOV     = 80,
    DEFAULT_HEIGHT_FOV    = 60, // --> to camera

    DEFAULT_SCREEN_WIDTH  = 1200,
    DEFAULT_SCREEN_HEIDHT = 900 // --> to screen
};

class Scene {
public:
    Scene(Config const &conf);
    void LoadSolid(std::string const &filename);

    void AskSolid();
    void RemoveSolid();
    void AskEquation();

    void Draw(Renderer &renderer);
    void HandleArgument(int argc, char *argv[]);

    Camera const* GetCamera() const;
    void TranslateCamera(Direction);
    void RotateCamera(Direction);
    void ToggleFrame();

private:
    Frame m_origin;
    Camera m_camera;

    std::vector<Light> m_lights;
    std::vector<Solid> m_solids;
};


}; // end namespace display3r

#endif // SCENE_H

