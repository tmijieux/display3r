#include "display3r/WindowMaker.hpp"
#include "display3r/Window.hpp"
#include "display3r/SharedLibrary.hpp"
#include "display3r/PluginDetails.hpp"
#include "display3r/Config.hpp"

using display3r::WindowMaker;
using display3r::Window;
using display3r::Config;
using namespace std;

Window *WindowMaker::MakeWindow(Config const &conf)
{
    string backendID = "backend."+conf.WindowBackend+".";
    string plugin_file = conf[backendID+"lib"].as<string>();
    cout << "plugin file: " << plugin_file << endl;

    shared_ptr<SharedLibrary> dll(
        new SharedLibrary(plugin_file, SharedLibraryFlag::LOCAL));

    m_dlls.push_back(dll);

    PluginDetails *info;
    info = reinterpret_cast<PluginDetails*>(dll->symbol("exports"));
    IWindowMaker *wm = info->initializeFunc();

    return wm->MakeWindow(conf.WindowWidth,
                          conf.WindowHeight,
                          conf.WindowBackground);
}
