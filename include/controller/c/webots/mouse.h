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

/************************************************************************/
/* Description:  Webots C programming interface for the computer mouse. */
/************************************************************************/

#ifndef WB_MOUSE_H
#define WB_MOUSE_H

#define WB_USING_C_API
#include <webots/mouse_state.h>
#include <webots/types.h>

#ifdef __cplusplus
//extern "C" {
#endif

CONTROLLER_EXPORT extern void wb_mouse_enable(WbRobotContext *context, int sampling_period);
CONTROLLER_EXPORT extern void wb_mouse_disable(WbRobotContext *context);
CONTROLLER_EXPORT extern int wb_mouse_get_sampling_period(WbRobotContext *context);

CONTROLLER_EXPORT extern void wb_mouse_enable_3d_position(WbRobotContext *context);
CONTROLLER_EXPORT extern void wb_mouse_disable_3d_position(WbRobotContext *context);
CONTROLLER_EXPORT extern bool wb_mouse_is_3d_position_enabled(WbRobotContext *context);

#ifndef WB_MATLAB_LOADLIBRARY
CONTROLLER_EXPORT extern WbMouseState wb_mouse_get_state(WbRobotContext *context);
#else
// This function should be used only in the Matlab wrapper
CONTROLLER_EXPORT extern WbMouseState *wb_mouse_get_state_pointer(WbRobotContext *context);
#endif

#ifdef __cplusplus
//}
#endif

#endif /* WB_MOUSE_H */
