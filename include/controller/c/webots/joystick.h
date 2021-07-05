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
//extern "C" {
#endif

CONTROLLER_EXPORT extern void wb_joystick_enable(WbRobotContext *context, int sampling_period);
CONTROLLER_EXPORT extern void wb_joystick_disable(WbRobotContext *context);
CONTROLLER_EXPORT extern int wb_joystick_get_sampling_period(WbRobotContext *context);

CONTROLLER_EXPORT extern bool wb_joystick_is_connected(WbRobotContext *context);
CONTROLLER_EXPORT extern const char *wb_joystick_get_model(WbRobotContext *context);

CONTROLLER_EXPORT extern int wb_joystick_get_number_of_axes(WbRobotContext *context);
CONTROLLER_EXPORT extern int wb_joystick_get_axis_value(WbRobotContext *context, int axis);

CONTROLLER_EXPORT extern int wb_joystick_get_number_of_povs(WbRobotContext *context);
CONTROLLER_EXPORT extern int wb_joystick_get_pov_value(WbRobotContext *context, int pov);

CONTROLLER_EXPORT extern int wb_joystick_get_pressed_button(WbRobotContext *context);

// force-feedback
CONTROLLER_EXPORT extern void wb_joystick_set_constant_force(WbRobotContext *context, int level);
CONTROLLER_EXPORT extern void wb_joystick_set_constant_force_duration(WbRobotContext *context, double duration);
CONTROLLER_EXPORT extern void wb_joystick_set_auto_centering_gain(WbRobotContext *context, double gain);
CONTROLLER_EXPORT extern void wb_joystick_set_resistance_gain(WbRobotContext *context, double gain);
CONTROLLER_EXPORT extern void wb_joystick_set_force_axis(WbRobotContext *context, int axis);

#ifdef __cplusplus
//}
#endif

#endif /* WB_JOYSTICK_H */
