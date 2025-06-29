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

#ifndef WB_CONTACT_SOUND_MANAGER_HPP
#define WB_CONTACT_SOUND_MANAGER_HPP

//
// Description: manage contact sounds
//

#include <QtCore/QList>

#include <core/WbConfig.h>

class WbOdeContact;

namespace WbContactSoundManager {
  WB_LIB_EXPORT void update(const QList<WbOdeContact> &odeContacts);
  WB_LIB_EXPORT void clearAllContactSoundSources();
};  // namespace WbContactSoundManager

#endif
