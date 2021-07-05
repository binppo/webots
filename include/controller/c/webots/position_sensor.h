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
/* Description:  Webots C programming interface for the Motor node                */
/**********************************************************************************/

#ifndef WB_POSITION_SENSOR_H
#define WB_POSITION_SENSOR_H

#define WB_USING_C_API
#include <webots/types.h>

#ifndef WB_MATLAB_LOADLIBRARY
#include <math.h>
#endif

#ifdef __cplusplus
//extern "C" {
#endif

CONTROLLER_EXPORT extern void wb_position_sensor_enable(WbRobotContext *context, WbDeviceTag tag, int sampling_period);  // milliseconds
CONTROLLER_EXPORT extern void wb_position_sensor_disable(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern int wb_position_sensor_get_sampling_period(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern double wb_position_sensor_get_value(WbRobotContext *context, WbDeviceTag tag);  // rad or meters
CONTROLLER_EXPORT extern WbJointType wb_position_sensor_get_type(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern WbDeviceTag wb_position_sensor_get_motor(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern WbDeviceTag wb_position_sensor_get_brake(WbRobotContext *context, WbDeviceTag tag);

#ifdef __cplusplus
//}
#endif

#endif /* WB_POSITION_SENSOR_H */
