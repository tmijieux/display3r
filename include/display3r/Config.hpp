#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "display3r/Color.hpp"

namespace display3r {

class Config{
public:
    std::string SceneCamera;
    std::string WindowBackend;
    int WindowWidth;
    int WindowHeight;

    Color WindowBackground;
    Color RendererUntextured;
    
    Config(int &argc, char **&argv);
private:

};

};
#endif //CONFIG_H
