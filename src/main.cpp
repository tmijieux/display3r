#include "display3r/Window.hpp"
#include "display3r/Config.hpp"
#include "display3r/Scene.hpp"
#include "display3r/Renderer.hpp"

using namespace display3r;

int main(int argc, char *argv[])
{
    Config conf(argc, argv);
    Window *window = Window::MakeWindow(conf);
    Scene scene(conf);
    Renderer renderer(window, conf.RendererUntextured);
    bool stop = false;

    // needed for initializing lenses stuffs: (zbuffer/dimensons/angles)
    scene.NotifyResize(window->GetWidth(), window->GetHeight());

    while (!stop) {
        window->Clear();
        scene.Clear();
        scene.Draw(renderer);
        window->Update();
        stop = window->HandleEvents(scene, renderer);
    }

    return EXIT_SUCCESS;
}
