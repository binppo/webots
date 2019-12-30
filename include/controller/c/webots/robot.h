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
/* Description:  Webots C programming interface for the Robot node                */
/**********************************************************************************/

#ifndef WB_ROBOT_H
#define WB_ROBOT_H

#define WB_USING_C_API
#include <webots/types.h>

#ifdef __CYGWIN__
#include <stdio.h>
#endif

#if defined(__VISUALC__) || defined(_MSC_VER)
#include "stdio.h"
#endif

#include "nodes.h"

#ifdef KROS_COMPILATION
#define main() _kros_main()
#endif

typedef void *WbMutexRef;  // identifier of a mutex

typedef enum { WB_MODE_SIMULATION = 0, WB_MODE_CROSS_COMPILATION, WB_MODE_REMOTE_CONTROL } WbRobotMode;

typedef enum {
  WB_EVENT_QUIT = -1,
  WB_EVENT_NO_EVENT = 0,
  WB_EVENT_MOUSE_CLICK = 1,
  WB_EVENT_MOUSE_MOVE = 2,
  WB_EVENT_KEYBOARD = 4,
  WB_EVENT_JOYSTICK_BUTTON = 8,
  WB_EVENT_JOYSTICK_AXIS = 16,
  WB_EVENT_JOYSTICK_POV = 32
} WbUserInputEvent;

// cart function headers
#ifdef __cplusplus
extern "C" {
#endif

CONTROLLER_EXPORT int wb_robot_init_common();

#if !defined(__VISUALC__) && !defined(_MSC_VER)
CONTROLLER_EXPORT int wb_robot_init();

/* In the visual studio case, the buffer size of the standard output and
 * the standard error cannot be modified from a dll
 */
#else
CONTROLLER_EXPORT int wb_robot_init();
#endif

CONTROLLER_EXPORT bool wb_is_robot_init();

CONTROLLER_EXPORT int wb_robot_step(int duration);  // milliseconds

#ifdef __CYGWIN__  // In that case, we need to flush explicitly the stdout/stdin streams otherwise they are buffered
// We cannot call fflush from the libController as libController is compiled with gcc8 and won't flush the stdout/stderr
// of a gcc7 (cygwin) compiled binary. Therefore, we need to perform the fflush in a gcc7 compiled code, e.g., in a macro here.
#define wb_robot_step(d) (fflush(NULL) ? wb_robot_step(d) : wb_robot_step(d))
#endif

CONTROLLER_EXPORT WbUserInputEvent wb_robot_wait_for_user_input_event(WbUserInputEvent event_type, int timeout);  // milliseconds
CONTROLLER_EXPORT void wb_robot_cleanup();
CONTROLLER_EXPORT double wb_robot_get_time();
CONTROLLER_EXPORT const char *wb_robot_get_name();
CONTROLLER_EXPORT const char *wb_robot_get_model();
CONTROLLER_EXPORT const char *wb_robot_get_custom_data();
CONTROLLER_EXPORT void wb_robot_set_custom_data(const char *data);
CONTROLLER_EXPORT WbRobotMode wb_robot_get_mode();
CONTROLLER_EXPORT void wb_robot_set_mode(WbRobotMode mode, void *args);
CONTROLLER_EXPORT bool wb_robot_get_synchronization();
CONTROLLER_EXPORT bool wb_robot_get_supervisor();
CONTROLLER_EXPORT const char *wb_robot_get_project_path();
CONTROLLER_EXPORT const char *wb_robot_get_world_path();
CONTROLLER_EXPORT double wb_robot_get_basic_time_step();
CONTROLLER_EXPORT WbDeviceTag wb_robot_get_device(const char *name);

// Controller API
CONTROLLER_EXPORT const char *wb_robot_get_controller_name();
CONTROLLER_EXPORT const char *wb_robot_get_controller_arguments();

// Introspection API
CONTROLLER_EXPORT int wb_robot_get_number_of_devices();
CONTROLLER_EXPORT WbDeviceTag wb_robot_get_device_by_index(int index);
CONTROLLER_EXPORT WbNodeType wb_robot_get_type();

// robot battery API
CONTROLLER_EXPORT void wb_robot_battery_sensor_enable(int sampling_period);
CONTROLLER_EXPORT void wb_robot_battery_sensor_disable();
CONTROLLER_EXPORT int wb_robot_battery_sensor_get_sampling_period();
CONTROLLER_EXPORT double wb_robot_battery_sensor_get_value();

// robot multi-thread API
#ifndef WB_MATLAB_LOADLIBRARY
CONTROLLER_EXPORT void wb_robot_task_new(void (*task)(void *), void *param);  // create a task
CONTROLLER_EXPORT WbMutexRef wb_robot_mutex_new();
CONTROLLER_EXPORT void wb_robot_mutex_lock(WbMutexRef);
CONTROLLER_EXPORT void wb_robot_mutex_unlock(WbMutexRef);
CONTROLLER_EXPORT void wb_robot_mutex_delete(WbMutexRef);
#endif

// Motion editor specfic function : Please don't use this function outside qt_utils
// This function doesn't work if the robot window has not been shown at lease once
CONTROLLER_EXPORT void wb_robot_pin_to_static_environment(bool pin);

// Deprecated functions
// deprecated since Webots 2018a, please use wb_robot_get_custom_data and
// wb_robot_set_custom_data instead
CONTROLLER_EXPORT const char *wb_robot_get_data() WB_DEPRECATED;
CONTROLLER_EXPORT void wb_robot_set_data(const char *data) WB_DEPRECATED;

#ifdef __cplusplus
}
#endif

#endif /* WB_ROBOT_H */
