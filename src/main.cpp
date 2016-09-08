#include "display3r/Window.hpp"
#include "display3r/Config.hpp"
#include "display3r/Scene.hpp"
#include "display3r/Renderer.hpp"

using namespace display3r;

int main(int argc, char *argv[])
{
    Config conf(argc, argv);
    Window *window = Window::MakeWindow(conf);
    Scene scene;
    Renderer renderer(scene.GetCamera(), window);
    bool stop = false;

    while (!stop) {
        
        renderer.Clear();
        scene.Draw(renderer);
        window->Update();
        
        stop = window->HandleEvents(scene, renderer);
    }

    return EXIT_SUCCESS;
}
