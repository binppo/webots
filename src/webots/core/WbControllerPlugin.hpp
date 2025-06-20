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

#ifndef WB_CONTROLLER_PLUGIN_HPP
#define WB_CONTROLLER_PLUGIN_HPP

#include <QtCore/QStringList>

#include <core/WbConfig.h>

namespace WbControllerPlugin {

  enum Type { ROBOT_WINDOW, REMOTE_CONTROL };

  // absolute filenames in the resources directory, the files doesn't exist necessarily
  WB_LIB_EXPORT const QStringList &defaultList(Type type);

  WB_LIB_EXPORT Type pluginSubDirectoryToType(const QString &pluginSubDirectory);

};  // namespace WbControllerPlugin

#endif
