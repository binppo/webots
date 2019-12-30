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
/* Description:  Webots C programming interface for the computer joystick         */
/**********************************************************************************/

#ifndef WB_JOYSTICK_H
#define WB_JOYSTICK_H

#define WB_USING_C_API
#include <webots/types.h>

#ifdef __cplusplus
extern "C" {
#endif

CONTROLLER_EXPORT void wb_joystick_enable(int sampling_period);
CONTROLLER_EXPORT void wb_joystick_disable();
CONTROLLER_EXPORT int wb_joystick_get_sampling_period();

CONTROLLER_EXPORT bool wb_joystick_is_connected();
CONTROLLER_EXPORT const char *wb_joystick_get_model();

CONTROLLER_EXPORT int wb_joystick_get_number_of_axes();
CONTROLLER_EXPORT int wb_joystick_get_axis_value(int axis);

CONTROLLER_EXPORT int wb_joystick_get_number_of_povs();
CONTROLLER_EXPORT int wb_joystick_get_pov_value(int pov);

CONTROLLER_EXPORT int wb_joystick_get_pressed_button();

// force-feedback
CONTROLLER_EXPORT void wb_joystick_set_constant_force(int level);
CONTROLLER_EXPORT void wb_joystick_set_constant_force_duration(double duration);
CONTROLLER_EXPORT void wb_joystick_set_auto_centering_gain(double gain);
CONTROLLER_EXPORT void wb_joystick_set_resistance_gain(double gain);
CONTROLLER_EXPORT void wb_joystick_set_force_axis(int axis);

#ifdef __cplusplus
}
#endif

#endif /* WB_JOYSTICK_H */
