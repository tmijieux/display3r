//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef DISPLAY3R_PLUGIN_DETAILS_H
#define DISPLAY3R_PLUGIN_DETAILS_H

namespace display3r {

// Forward declare the plugin class which must be defined externally.
class IWindowMaker;

// Define the API version.
// This value is incremented whenever there are ABI breaking changes.
#define DISPLAY3R_PLUGIN_API_VERSION 1

#ifdef WIN32
# define DISPLAY3R_PLUGIN_EXPORT __declspec(dllexport)
#else
# define DISPLAY3R_PLUGIN_EXPORT // empty
#endif
#define DISPLAY3R_EXTERN extern "C"

// Define a type for the static function pointer.
DISPLAY3R_EXTERN typedef IWindowMaker* (*GetPluginFunc)();

// Plugin details structure that's exposed to the application.
struct PluginDetails {
    int apiVersion;
    const char* fileName;
    const char* className;
    const char* pluginName;
    const char* pluginVersion;
    GetPluginFunc initializeFunc;
};

#define DISPLAY3R_STANDARD_PLUGIN_STUFF \
    DISPLAY3R_PLUGIN_API_VERSION,       \
    __FILE__

#define DISPLAY3R_PLUGIN(classType, pluginName, pluginVersion)     \
  extern "C" {                                               \
      DISPLAY3R_PLUGIN_EXPORT display3r::IWindowMaker* getPlugin()     \
      {                                                      \
          static classType singleton;                        \
          return &singleton;                                 \
      }                                                      \
      DISPLAY3R_PLUGIN_EXPORT display3r::PluginDetails exports =  \
      {                                                      \
          DISPLAY3R_STANDARD_PLUGIN_STUFF,                         \
          #classType,                                        \
          pluginName,                                        \
          pluginVersion,                                     \
          getPlugin,                                         \
      };                                                     \
  }

}; //namespace display3r


#endif // DISPLAY3R_PLUGIN_DETAILS_H
