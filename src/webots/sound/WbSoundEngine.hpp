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

#ifndef WB_SOUND_ENGINE_HPP
#define WB_SOUND_ENGINE_HPP

#include <core/WbConfig.h>

//
// Description: manage sounds
//

class WbWorld;
class QString;
class WbSoundClip;
class WbSoundSource;

namespace WbSoundEngine {
  WB_LIB_EXPORT void setWorld(WbWorld *world);
  WB_LIB_EXPORT void setMute(bool mute);
  WB_LIB_EXPORT void setVolume(int volume);
  WB_LIB_EXPORT void setPause(bool pause);
  WB_LIB_EXPORT void updateAfterPhysicsStep();
  WB_LIB_EXPORT void updateListener();
  WB_LIB_EXPORT WbSoundSource *createSource();
  WB_LIB_EXPORT void deleteSource(WbSoundSource *);
  WB_LIB_EXPORT void stopAllSources();
  // 0: both sides, -1: left only, 1: right only
  WB_LIB_EXPORT WbSoundClip *sound(const QString &filename, double balance = 0.0, int side = 0);
  WB_LIB_EXPORT WbSoundClip *soundFromText(const QString &text, const QString &engine, const QString &language);
  WB_LIB_EXPORT void clearAllMotorSoundSources();
  WB_LIB_EXPORT void clearAllContactSoundSources();
};  // namespace WbSoundEngine

#endif
