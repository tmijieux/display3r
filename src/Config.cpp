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

void Config::ConfigDescription(po::options_description& config)
{
    for (auto &c : m_cameras) {
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

void Config::ParseConfigFile()
{
    ifstream ifs(m_config_filename);
    if (!ifs.is_open())
        throw runtime_error(
            "Cannot open '"+m_config_filename+"'. Aborting.");
    ParseConfigFile(ifs);
}

void Config::ParseConfigFile(istream &ifs)
{
    po::options_description config("3Displayer configuration");
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
    ConfigDescription(config);
    store(parse_config_file(ifs, config), m_config_vm);
    notify(m_config_vm);
}

void Config::CommandLineDescription(po::options_description &desc)
{
    desc.add_options()
        ("help,h", "print this help message")

        ("config,f",po::value<string>(&m_config_filename)
         ->default_value("./config.ini")
         , "set the config file")

        ("list,l", po::value<string>(&m_match_pattern)
         ->implicit_value(""),
         "list the available objects and exit")

        ("show,s", po::value<string>(&m_match_pattern)
         ->implicit_value(""),
         "show the object values")

        ("camera,c", po::value<string>()->default_value("standard")
         , "choose the camera type")

        ("backend,b", po::value<string>(),
         "choose the backend for the window system")

        ("write,w", po::value<string>(&m_output_config_filename),
         "create an example config file from default values")
        ;
}

Config::Config(int &argc, char **&argv)
{
    po::variables_map vm;
    po::options_description desc;

    CommandLineDescription(desc);
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) { cout << desc; exit(EXIT_SUCCESS); }
    if (vm.count("write")) WriteDefaultConfigFile();

    po::notify(vm);
    ParseConfigFile();

    if (vm.count("list"))    ListObjects();
    if (vm.count("show"))    ShowObjects();
    if (vm.count("camera"))  SceneCamera = vm["camera"].as<string>();
    if (vm.count("backend")) WindowBackend = vm["backend"].as<string>();

    WindowBackground = Color::FromString(
        (*this)["window.background"].as<string>());
    RendererUntextured = Color::FromString(
        (*this)["renderer.untextured"].as<string>());
}

static void
PrintSetMatching(string name, set<string> const &s, string match)
{
    std::string match_notice = "";
    if (match != "")
        match_notice  = " matching '"+match+"'";
    cout << "Available " << name << match_notice << ":" << endl;

    for (auto &i : s)
        if (i.find(match) != string::npos)
            cout << "  " << i << endl;
    cout << endl;
}

void Config::ListObjects()
{
    PrintSetMatching("backends", m_backends, m_match_pattern);
    PrintSetMatching("cameras", m_cameras, m_match_pattern);
    PrintSetMatching("lenses", m_lenses, m_match_pattern);
    PrintSetMatching("lights", m_lights, m_match_pattern);
    exit(EXIT_SUCCESS);
}

static void
PrintVariableMatching(po::variables_map const &vm,
                      std::string const &pattern)
{
    for (auto &i  : vm) {
        if (i.first.find(pattern) != string::npos) {
            std::cout << i.first << "=";
            auto& value = i.second.value();
            if (auto v = boost::any_cast<float>(&value))
                cout << *v;
            else if (auto v = boost::any_cast<int>(&value))
                cout << *v;
            else if (auto v = boost::any_cast<string>(&value))
                cout << *v;
            else if (auto v = boost::any_cast<bool>(&value))
                cout << *v;
            else if (auto v = boost::any_cast<vector<string> >(&value))
                for (auto &i : *v)
                    cout << i << " ";
            else
                cout << "<CANNOT DISPLAY VALUE>";
            cout << endl;
        }
    }
}

void Config::ShowObjects()
{
    PrintVariableMatching(m_config_vm, m_match_pattern);
    exit(EXIT_SUCCESS);
}

po::variable_value const &Config::operator[](std::string key) const
{
    if (!m_config_vm.count(key))
        boost::throw_exception(po::required_option(key));
    return m_config_vm[key];
}

void Config::WriteDefaultConfigFile()
{
    ofstream f(m_output_config_filename);
    if (!f.is_open())
        throw runtime_error(
            "Cannot open file '"+m_output_config_filename+"'");
    WriteDefaultConfigFile(f);
    exit(EXIT_SUCCESS);
}

void Config::WriteDefaultConfigFile(ostream &out)
{
    out <<
	"[camera.stereo]" << endl <<
	"translationSpeed=0.1" << endl <<
	"rotationSpeed=0.01" << endl <<
	"position=0. -5. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"lens=stereo.left" << endl <<
	"lens=stereo.right" << endl <<
	"" << endl <<
	"[lens.stereo.left]" << endl <<
	"offset=-0.5 0. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=0" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=4" << endl <<
	"screenHeight=8" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.stereo.right]" << endl <<
	"offset=0.5 0. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=4" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=4" << endl <<
	"screenHeight=8" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.standard]" << endl <<
	"offset=0. 0. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=0" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=8" << endl <<
	"screenHeight=8" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=80" << endl <<
	"" << endl <<
	"[camera.ncurses]" << endl <<
	"translationSpeed=0.1" << endl <<
	"rotationSpeed=0.1" << endl <<
	"position=0. -5. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"lens=standard" << endl <<
	"" << endl <<
	"[camera.spider]" << endl <<
	"translationSpeed=0.1" << endl <<
	"rotationSpeed=0.01" << endl <<
	"position=0. -5. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"lens=spider.1" << endl <<
	"lens=spider.2" << endl <<
	"lens=spider.3" << endl <<
	"lens=spider.4" << endl <<
	"lens=spider.5" << endl <<
	"lens=spider.6" << endl <<
	"lens=spider.7" << endl <<
	"lens=spider.8" << endl <<
	"" << endl <<
	"" << endl <<
	"[lens.spider.1]" << endl <<
	"offset=-0.75 0. 0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=0" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.spider.2]" << endl <<
	"offset=-0.25 0. 0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=2" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.spider.3]" << endl <<
	"offset=0.25 0. 0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=4" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.spider.4]" << endl <<
	"offset=0.75 0. 0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=6" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.spider.5]" << endl <<
	"offset=-0.75 0. -0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=0" << endl <<
	"screenPositionHeight=4" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.spider.6]" << endl <<
	"offset=-0.25 0. -0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=2" << endl <<
	"screenPositionHeight=4" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.spider.7]" << endl <<
	"offset=0.25 0. -0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=4" << endl <<
	"screenPositionHeight=4" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[lens.spider.8]" << endl <<
	"offset=0.75 0. -0.25" << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 255 255" << endl <<
	"screenPositionWidth=6" << endl <<
	"screenPositionHeight=4" << endl <<
	"screenWidth=2" << endl <<
	"screenHeight=4" << endl <<
	"overlapping=0" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=90" << endl <<
	"" << endl <<
	"[camera.standard]" << endl <<
	"translationSpeed=0.1" << endl <<
	"rotationSpeed=0.01" << endl <<
	"position=0. -5. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"lens=standard" << endl <<
	"" << endl <<
	"[camera.bicolor]" << endl <<
	"translationSpeed=0.1" << endl <<
	"rotationSpeed=0.01" << endl <<
	"position=0. -5. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"lens=bicolor.red_left" << endl <<
	"lens=bicolor.blue_right" << endl <<
	"" << endl <<
	"[lens.bicolor.red_left]" << endl <<
	"offset=-0.1 0. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=255 0 0" << endl <<
	"screenPositionWidth=0" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=8" << endl <<
	"screenHeight=8" << endl <<
	"overlapping=1" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=60" << endl <<
	"" << endl <<
	"[lens.bicolor.blue_right]" << endl <<
	"offset=0.1 0. 0." << endl <<
	"theta=0." << endl <<
	"phi=0." << endl <<
	"rho=0." << endl <<
	"filter=0 255 255" << endl <<
	"screenPositionWidth=0" << endl <<
	"screenPositionHeight=0" << endl <<
	"screenWidth=8" << endl <<
	"screenHeight=8" << endl <<
	"overlapping=1" << endl <<
	"nearplan=1." << endl <<
	"farplan=20." << endl <<
	"wfov=60" << endl <<
	"" << endl <<
	"[light.red_sphere]" << endl <<
	"position=0 0.58 2" << endl <<
	"direction=0. 0. -1." << endl <<
	"intensity=2." << endl <<
	"inner=360" << endl <<
	"outer=360" << endl <<
	"color=255 0 0" << endl <<
	"" << endl <<
	"[light.blue_sphere]" << endl <<
	"position=-0.5 -0.29 2" << endl <<
	"direction=0. 0. -1." << endl <<
	"intensity=2." << endl <<
	"inner=360" << endl <<
	"outer=360" << endl <<
	"color=0 0 255" << endl <<
	"" << endl <<
	"[light.green_sphere]" << endl <<
	"position=0.5 -0.29 2" << endl <<
	"direction=0. 0. -1." << endl <<
	"intensity=2." << endl <<
	"inner=360" << endl <<
	"outer=360" << endl <<
	"color=0 255 0" << endl <<
	"" << endl <<
	"[light.red]" << endl <<
	"position=0 0.58 2" << endl <<
	"direction=0. 0. -1." << endl <<
	"intensity=2." << endl <<
	"inner=30" << endl <<
	"outer=45" << endl <<
	"color=255 0 0" << endl <<
	"" << endl <<
	"[light.blue]" << endl <<
	"position=-0.5 -0.29 2" << endl <<
	"direction=0. 0. -1." << endl <<
	"intensity=2." << endl <<
	"inner=30" << endl <<
	"outer=45" << endl <<
	"color=0 0 255" << endl <<
	"" << endl <<
	"[light.green]" << endl <<
	"position=0.5 -0.29 2" << endl <<
	"direction=0. 0. -1." << endl <<
	"intensity=2." << endl <<
	"inner=30" << endl <<
	"outer=45" << endl <<
	"color=0 255 0" << endl <<
	"" << endl <<
	"[light.standard]" << endl <<
	"position=-2. -2. 3." << endl <<
	"direction=0.75 1. -2." << endl <<
	"intensity=1." << endl <<
	"inner=-1" << endl <<
	"outer=-1" << endl <<
	"color=255 255 255" << endl <<
	"" << endl <<
	"[light.opposed]" << endl <<
	"position=2. 2. -3." << endl <<
	"direction=-0.75 -1. 2." << endl <<
	"intensity=1." << endl <<
	"inner=-1" << endl <<
	"outer=-1" << endl <<
	"color=255 255 255" << endl <<
	"" << endl <<
	"[backend.SDL]" << endl <<
	"lib=SDL_3displayer" << endl <<
	"" << endl <<
	"[backend.ncurses]" << endl <<
	"lib=ncurses_3displayer" << endl <<
	"" << endl <<
	"[window]" << endl <<
	"width=1024" << endl <<
	"height=768" << endl <<
	"background=140 140 140" << endl <<
	"backend=SDL" << endl <<
	"" << endl <<
	"[scene]" << endl <<
	"camera=standard" << endl <<
	"light=standard" << endl <<
	"#light can be specified multiple times" << endl <<
	"" << endl <<
	"[renderer]" << endl <<
	"untextured=100 100 100" << endl;

}
