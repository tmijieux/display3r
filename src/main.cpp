#include <iostream>
#include <exception>
#include <memory>

#include "display3r/Window.hpp"
#include "display3r/WindowMaker.hpp"
#include "display3r/Config.hpp"
#include "display3r/Scene.hpp"
#include "display3r/Renderer.hpp"

using namespace display3r;
using namespace std;

int main(int argc, char *argv[])
{
    try {
        Config conf(argc, argv);
        WindowMaker WM;
        unique_ptr<Window> window(WM.MakeWindow(conf));
        Scene scene(conf);
        Renderer renderer(window.get(), conf.RendererUntextured);
        bool stop = false;

        /* following is needed for initializing lenses stuffs:
           (zbuffer/dimensions/fovs) */
        scene.NotifyResize(window->GetWidth(), window->GetHeight());

        while (!stop) {
            window->Clear();
            scene.Clear();
            scene.Draw(renderer);
            window->Update();
            stop = window->HandleEvents(scene, renderer);
        }
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
