#include <boost/filesystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <iostream>

#include <fstream>
#include <iterator>
#include <cstdlib>
#include "display3r/Config.hpp"

using namespace std;
using namespace boost::algorithm;
using display3r::Config;

void Config::AddOptionDescriptions(po::options_description& config)
{
    for (auto &c : m_cameras) {
        cout << "adding camera '" << c << "'" << endl;
        config.add_options()
            (("camera."+c+".translationSpeed").c_str(),  po::value<float>(),
             "camera translation speed")
            (("camera."+c+".rotationSpeed").c_str(),  po::value<float>(),
             "camera rotation speed")
                    
            (("camera."+c+".position").c_str(), po::value<string>(),
             "camera position")
                    
            (("camera."+c+".theta").c_str(), po::value<float>(),
             "camera angular position around x axis")
                    
            (("camera."+c+".phi").c_str(), po::value<float>(), 
             "camera angular position around y axis")
                    
            (("camera."+c+".rho").c_str(), po::value<float>(),
             "camera angular position around z axis")
                    
            (("camera."+c+".lens").c_str(),
             po::value<vector<string> >()->multitoken(), 
             "camera 's lens(es)")
            ;
    }

    for (auto &l : m_lenses) {
        cout << "adding lens '" << l << "'" << endl;
        config.add_options()
            (("lens."+l+".offset").c_str(), po::value<string>(),
             "position offset relative to parent camera")
                    
            (("lens."+l+".theta").c_str(), po::value<float>(),
             "angle around x axis")
                    
            (("lens."+l+".phi").c_str(), po::value<float>(),
             "angle around y axis")
                    
            (("lens."+l+".rho").c_str(), po::value<float>(),
             "angle around z axis")
                    
            (("lens."+l+".filter").c_str(), po::value<string>(),
             "lens color")
                    
            (("lens."+l+".screenPositionWidth").c_str(), po::value<int>(),
             "lens sub-window position along x axis")
                    
            (("lens."+l+".screenPositionHeight").c_str(), po::value<int>(),
             "lens sub-window position along y axis")
                
            (("lens."+l+".screenWidth").c_str(), po::value<int>(),
             "lens sub-window width")
                    
            (("lens."+l+".screenHeight").c_str(), po::value<int>(),
             "lens sub-window height")
                    
            (("lens."+l+".overlapping").c_str(), po::value<bool>(), 
             "whether lens window overlap with other window or not?")
                    
            (("lens."+l+".nearplan").c_str(), po::value<float>(),
             "nearplan, objects before near plan are not drawn")
            (("lens."+l+".farplan").c_str(), po::value<float>(),
             "farplan, objects beyond far plan are not drawn")
                    
            (("lens."+l+".wfov").c_str(), po::value<float>(),
             "field of view (width)")
            ;
    }

    for (auto &l : m_lights) {
        cout << "adding light '" << l << "'" << endl;
        config.add_options()
            (("light."+l+".position").c_str(), po::value<string>(),
             "light position")
            (("light."+l+".direction").c_str(), po::value<string>(),
             "light sport direction")
            (("light."+l+".intensity").c_str(), po::value<float>(),
             "light intensity")
            (("light."+l+".inner").c_str(), po::value<float>(),
             "light sport inner angle")
            (("light."+l+".outer").c_str(), po::value<float>(),
             "light spot outer angle")
            (("light."+l+".color").c_str(), po::value<string>(),
             "light color")
            ;
    }
    for (auto &b : m_backends) {
        cout << "adding backend '" << b << "'" << endl;
        config.add_options()
            (("backend."+b+".lib").c_str(), po::value<string>(),
             "library file for window backend plugin")
            ;
    }

    config.add_options()
        ("window.width", po::value<int>(&WindowWidth), "window width")
        ("window.height", po::value<int>(&WindowHeight), "window height")
        ("window.background", po::value<string>(),
         "window default background color")
        ("window.backend", po::value<string>(&WindowBackend),
         "window backend plugin")
        ;

    config.add_options()
        ("scene.camera", po::value<string>(&SceneCamera), "scene camera")
        ("scene.light", po::value<vector<string> >()->multitoken(),
         "scene light")
        ;

    config.add_options()
        ("renderer.untextured", po::value<string>(),
         "color for drawing faces with no textures")
        ;
}

void Config::ParseConfigFile(istream &ifs)
{
    try {
        po::options_description config("3Displayer configuration");
        
        // Scan the configuration file to find all the used databases.
        {
            po::options_description inner_config("Configuration");
            inner_config.add_options()
                ("camera.*", po::value<string>(), "")
                ("lens.*", po::value<string>(), "")
                ("light.*", po::value<string>(), "")
                ("backend.*", po::value<string>(), "")
                ;

            po::parsed_options p = parse_config_file(ifs, inner_config, true);
            
            ifs.clear(); 
            ifs.seekg(0);//rewind

            for (size_t i = 0; i < p.options.size(); ++i) {
                auto s = p.options[i].string_key.find_first_of(".");
                auto t = p.options[i].string_key.find_last_of(".");
                
                if (s != string::npos && t != string::npos) {
                    string type = p.options[i].string_key.substr(0, s);
                    string value = p.options[i].string_key.substr(s+1, t-s-1);
                    cout << "value: " << value << endl;
                    if (type == "lens")
                        m_lenses.insert(value);
                    if (type == "camera")
                        m_cameras.insert(value);
                    if (type == "light")
                        m_lights.insert(value);
                    if (type == "backend")
                        m_backends.insert(value);
                }
            }
        }
        AddOptionDescriptions(config);
        
        store(parse_config_file(ifs, config), *this);
        notify();

    } catch(exception &e) {
        cout <<"rrooor!" << endl;
        cout << e.what() << endl;
    }
}


Config::Config(int &argc, char **&argv):
    SceneCamera("camera/camera.txt"),
    WindowBackend("SDL_backend"),
    WindowWidth(1024),
    WindowHeight(768),
    WindowBackground(127, 127, 127),
    RendererUntextured(255,255,255)
{
    ifstream ifs("config.ini");
    if (!ifs.is_open()) {
        cerr << "Cannot open 'config.ini'. Aborting." << endl;
        exit(EXIT_FAILURE);
    }
    
    ParseConfigFile(ifs);
    
}
