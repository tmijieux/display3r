#include "display3r/WindowMaker.hpp"
#include "display3r/Window.hpp"
#include "display3r/PluginDetails.hpp"
#include "display3r/Config.hpp"

using namespace display3r;
using namespace std;

namespace dll = boost::dll;

Window *WindowMaker::MakeWindow(Config const &conf)
{
    string backendID = "backend."+conf.WindowBackend+".";
    string plugin_file = conf[backendID+"lib"].as<string>();
    cout << "plugin file: " << plugin_file << endl;

    dll::shared_library lib(plugin_file, dll::load_mode::append_decorations);
    boost::shared_ptr<PluginDetails> info;
    info = dll::import<PluginDetails>(lib, "exports");
    IWindowMaker *wm = info->initializeFunc();

    m_dlls.push_back(std::move(lib));
    return wm->MakeWindow(conf.WindowWidth,
                          conf.WindowHeight,
                          conf.WindowBackground);
}
