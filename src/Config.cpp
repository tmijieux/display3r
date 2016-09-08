#include "display3r/Config.hpp"

using display3r::Config;

Config::Config(int &argc, char **&argv):
    SceneCamera("camera/camera.txt"),
    WindowBackend("SDL_backend"),
    WindowWidth(1024),
    WindowHeight(768),
    WindowBackground(127, 127, 127),
    RendererUntextured(255,255,255)
{
    
}
