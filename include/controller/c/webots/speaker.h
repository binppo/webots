/*
 * Copyright 1996-2019 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**********************************************************************************/
/* Description:  Webots C programming interface for the Speaker node              */
/**********************************************************************************/

#ifndef WB_SPEAKER_H
#define WB_SPEAKER_H

#define WB_USING_C_API
#include <webots/types.h>

#ifdef __cplusplus
//extern "C" {
#endif

CONTROLLER_EXPORT extern void wb_speaker_play_sound(WbRobotContext *context, WbDeviceTag left, WbDeviceTag right, const char *sound, double volume, double pitch, double balance,
                           bool loop);
CONTROLLER_EXPORT extern void wb_speaker_stop(WbRobotContext *context, WbDeviceTag tag, const char *sound);
CONTROLLER_EXPORT extern bool wb_speaker_is_sound_playing(WbRobotContext *context, WbDeviceTag tag, const char *sound);

// "pico" or "microsoft" (Windows only)
CONTROLLER_EXPORT extern bool wb_speaker_set_engine(WbRobotContext *context, WbDeviceTag tag, const char *engine);
// "en-US", "en-UK", "de-DE", "es-ES", "fr-FR", "it-IT", etc.
CONTROLLER_EXPORT extern bool wb_speaker_set_language(WbRobotContext *context, WbDeviceTag tag, const char *language);
CONTROLLER_EXPORT extern const char *wb_speaker_get_engine(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern const char *wb_speaker_get_language(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern void wb_speaker_speak(WbRobotContext *context, WbDeviceTag tag, const char *text, double volume);
CONTROLLER_EXPORT extern bool wb_speaker_is_speaking(WbRobotContext *context, WbDeviceTag tag);

#ifdef __cplusplus
//}
#endif

#endif /* WB_SPEAKER_H */
