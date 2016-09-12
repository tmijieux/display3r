#ifndef DISPLAY3R_CONFIG_H
#define DISPLAY3R_CONFIG_H

#include <string>
#include <set>
#include <boost/program_options.hpp>
#include <iostream>
#include "display3r/Color.hpp"

namespace po = boost::program_options;
namespace display3r {

class Config {
public /*methods*/:
    Config(int &argc, char **&argv);
    po::variable_value const &operator[](std::string key) const;

public /*attributes*/:
    std::string SceneCamera;
    std::string WindowBackend;
    Color WindowBackground;
    Color RendererUntextured;
    int WindowWidth;
    int WindowHeight;

private /*methods*/:
    void ParseConfigFile();
    void ParseConfigFile(std::istream &ifs);

    void WriteDefaultConfigFile();
    void WriteDefaultConfigFile(std::ostream &out);

    void ListObjects();
    void ShowObjects();

    void ConfigDescription(po::options_description&);
    void CommandLineDescription(po::options_description&);

private /*attributes*/:
    std::set<std::string> m_backends;
    std::set<std::string> m_lenses;
    std::set<std::string> m_cameras;
    std::set<std::string> m_lights;

    po::variables_map m_config_vm;
    std::string m_match_pattern;
    std::string m_config_filename;
    std::string m_output_config_filename;
};

};

#endif //DISPLAY3R_CONFIG_H
