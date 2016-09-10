#ifndef DISPLAY3R_CONFIG_H
#define DISPLAY3R_CONFIG_H

#include <string>
#include <set>
#include <boost/program_options.hpp>

#include "display3r/Color.hpp"

namespace po = boost::program_options;

namespace display3r {

class Config : public po::variables_map {
public:
    std::string SceneCamera;
    std::string WindowBackend;
    int WindowWidth;
    int WindowHeight;
    Color WindowBackground;
    Color RendererUntextured;

    Config(int &argc, char **&argv);

private:
    std::set<std::string> m_backends;
    std::set<std::string> m_lenses;
    std::set<std::string> m_cameras;
    std::set<std::string> m_lights;

    void ParseConfigFile(std::istream &ifs);
    void AddConfigDescriptions(po::options_description&);
};

};

#endif //DISPLAY3R_CONFIG_H
