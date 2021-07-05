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

#ifndef ROBOT_PRIVATE_H
#define ROBOT_PRIVATE_H

#include <webots/supervisor.h>
#include <webots/types.h>
#include <string>
#include <vector>
#include "device_private.h"
#include "request.h"
#include "webots/nodes.h"

#define ROBOT_ASSERT(context, condition)                                                     \
  {                                                                                 \
    if (!(condition))                                                               \
      robot_abort(context, "%s:%d: assertion failed: %s\n", __FILE__, __LINE__, #condition); \
  }

struct UpdateElement;
struct GPipe;
struct Label;

int wb_robot_get_step_duration(WbRobotContext *context);
unsigned int wb_robot_get_dongle_number();
void wb_robot_flush_unlocked(WbRobotContext *context);
void robot_write_request(WbRobotContext *, WbDeviceStruct *, WbRequest *);
void robot_read_answer(WbRobotContext *, WbDeviceStruct *, WbRequest *);
WbDeviceStruct *robot_get_device_with_node(WbRobotContext *context, WbDeviceTag tag, WbNodeType node, bool warning);
int robot_get_number_of_devices(WbRobotContext *context);
WbDeviceTag robot_get_device_tag(WbRobotContext *context, WbDeviceStruct *);
WbDeviceStruct *robot_get_robot_device(WbRobotContext *context);
double wb_robot_get_time(WbRobotContext *context);
int robot_check_supervisor(WbRobotContext *context, const char *func_name);
int robot_check_differential_wheels(WbRobotContext *context, const char *func_name);
const char *robot_get_device_name(WbRobotContext *context, WbDeviceTag tag);
const char *robot_get_device_model(WbRobotContext *context, WbDeviceTag tag);
void robot_mutex_lock_step(WbRobotContext *context);
void robot_mutex_unlock_step(WbRobotContext *context);
void robot_abort(WbRobotContext *context, const char *format, ...);
WbNodeType robot_get_device_type(WbRobotContext *context, WbDeviceTag tag);
void robot_toggle_remote(WbRobotContext *context, WbDeviceStruct *, WbRequest *);
int robot_is_quitting(WbRobotContext *context);
void robot_console_print(WbRobotContext *context, const char *text, int stream);
WbSimulationMode robot_get_simulation_mode(WbRobotContext *context);
void robot_set_simulation_mode(WbRobotContext *context, WbSimulationMode mode);

#endif  // ROBOT_PRIVATE_H
