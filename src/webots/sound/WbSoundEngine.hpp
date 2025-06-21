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

#ifndef WB_SOUND_ENGINE_HPP
#define WB_SOUND_ENGINE_HPP

//
// Description: manage sounds
//

#include <core/WbConfig.h>

class QString;
class QIODevice;

class WbWorld;
class WbSoundClip;
class WbSoundSource;

namespace WbSoundEngine {
  WB_LIB_EXPORT bool openAL();
  WB_LIB_EXPORT const QString &device();
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
  WB_LIB_EXPORT WbSoundClip *sound(const QString &url, const QString &extension, QIODevice *device = 0, double balance = 0.0, int side = 0);
  WB_LIB_EXPORT WbSoundClip *soundFromText(const QString &text, const QString &engine, const QString &language);
  WB_LIB_EXPORT void clearAllMotorSoundSources();
  WB_LIB_EXPORT void clearAllContactSoundSources();
  WB_LIB_EXPORT void updateViewpointConnection();

};  // namespace WbSoundEngine

#endif
