// Copyright 1996-2019 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WB_CONTEXT_MENU_GENERATOR_HPP
#define WB_CONTEXT_MENU_GENERATOR_HPP

//
// Description: namespace for generating scene tree and view3d context menus
//

#include <QtCore/QPoint>

#include <core/WbConfig.h>

class QMenu;
class WbBaseNode;

namespace WbContextMenuGenerator {
  WB_LIB_EXPORT void generateContextMenu(const QPoint &position, const WbBaseNode *selectedNode);
  WB_LIB_EXPORT void enableNodeActions(bool enabled);
  WB_LIB_EXPORT void enableProtoActions(bool enabled);
  WB_LIB_EXPORT void enableRobotActions(bool enabled);
  WB_LIB_EXPORT void setRobotCameraMenu(QMenu *menu);
  WB_LIB_EXPORT void setRobotRangeFinderMenu(QMenu *menu);
  WB_LIB_EXPORT void setRobotDisplayMenu(QMenu *menu);
  WB_LIB_EXPORT void setUserMenu(QMenu *menu);
};  // namespace WbContextMenuGenerator

#endif
