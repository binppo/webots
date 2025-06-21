// Copyright 1996-2024 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WB_STANDARD_PATHS_HPP
#define WB_STANDARD_PATHS_HPP

#include <core/WbConfig.h>

class QString;

namespace WbStandardPaths {
  // directories, every path is terminated by a /
  WB_LIB_EXPORT const QString &webotsHomePath();     // e.g. /usr/local/webots/
  WB_LIB_EXPORT const QString &webotsLibPath();      // e.g. /usr/local/webots/lib/webots/
  WB_LIB_EXPORT const QString &controllerLibPath();  // e.g. /usr/local/webots/lib/controller/
#ifdef _WIN32
  WB_LIB_EXPORT const QString &webotsMsys64Path();  // e.g. C:/Program Files/Webots/msys64/
#endif
  WB_LIB_EXPORT const QString &localDocPath();                      // e.g. /usr/local/webots/docs/
  WB_LIB_EXPORT const QString &projectsPath();                      // e.g. /usr/local/webots/projects/
  WB_LIB_EXPORT const QString &resourcesPath();                     // e.g. /usr/local/webots/resources/
  WB_LIB_EXPORT const QString &translationsPath();                  // e.g. /usr/local/webots/resources/translations/
  WB_LIB_EXPORT const QString &templatesPath();                     // e.g. /usr/local/webots/resources/templates/
  WB_LIB_EXPORT const QString &fontsPath();                         // e.g. /usr/local/webots/resources/fonts/
  WB_LIB_EXPORT const QString &resourcesPicoPath();                 // e.g. /usr/local/webots/resources/pico/lang/
  WB_LIB_EXPORT const QString &resourcesProjectsPath();             // e.g. /usr/local/webots/resources/projects/
  WB_LIB_EXPORT const QString &resourcesControllersPath();          // e.g. /usr/local/webots/resources/projects/controllers/
  WB_LIB_EXPORT const QString &resourcesPhysicsPluginsPath();       // e.g. /usr/local/webots/resources/projects/plugins/physics/
  WB_LIB_EXPORT const QString &resourcesSoundPluginsPath();         // e.g. /usr/local/webots/resources/projects/plugins/sound/
  WB_LIB_EXPORT const QString &resourcesRobotWindowsPluginsPath();  // e.g. /usr/local/webots/resources/projects/plugins/robot_windows/
  WB_LIB_EXPORT const QString &resourcesWebPath();                  // e.g. /usr/local/webots/resources/web/

  WB_LIB_EXPORT const QString &emptyProjectPath();  // equal to resourcesProjectsPath() if WEBOTS_EMPTY_PROJECT_PATH is not set

  // urls
  WB_LIB_EXPORT const QString &cyberboticsUrl();       // https://cyberbotics.com
  WB_LIB_EXPORT const QString &githubRepositoryUrl();  // https://github.com/cyberbotics/webots

  // utility function
  WB_LIB_EXPORT const QString &dynamicLibraryExtension();  // e.g. .so, .dll or .dylib
  WB_LIB_EXPORT const QString &dynamicLibraryPrefix();     // e.g. lib or ''
  WB_LIB_EXPORT const QString &executableExtension();      // e.g. .exe or ''

  // file names
  WB_LIB_EXPORT const QString &unnamedTextFile();  // "unnamed.txt"

  // temporary directory, e.g. /tmp/webots-<id>/ where <id> corresponds to the TCP port
  WB_LIB_EXPORT bool webotsTmpPathCreate(const int id);
  WB_LIB_EXPORT int webotsTmpPathId();
  WB_LIB_EXPORT const QString &webotsTmpPath();

  // cached assets directory
  WB_LIB_EXPORT const QString &cachedAssetsPath();

  // other locations
  WB_LIB_EXPORT const QString &vehicleLibraryPath();

};  // namespace WbStandardPaths

#endif
