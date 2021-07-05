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
/* Description:  Webots C programming interface for the DifferentialWheels node   */
/**********************************************************************************/

#ifndef WB_DIFFERENTIAL_WHEELS_H
#define WB_DIFFERENTIAL_WHEELS_H

#define WB_USING_C_API
#include <webots/types.h>

#ifdef __cplusplus
//extern "C" {
#endif

CONTROLLER_EXPORT extern void wb_differential_wheels_set_speed(WbRobotContext *context, double left, double right);
CONTROLLER_EXPORT extern double wb_differential_wheels_get_left_speed(WbRobotContext *context);
CONTROLLER_EXPORT extern double wb_differential_wheels_get_right_speed(WbRobotContext *context);
CONTROLLER_EXPORT extern double wb_differential_wheels_get_max_speed(WbRobotContext *context);
CONTROLLER_EXPORT extern double wb_differential_wheels_get_speed_unit(WbRobotContext *context);

CONTROLLER_EXPORT extern void wb_differential_wheels_enable_encoders(WbRobotContext *context, int sampling_period);
CONTROLLER_EXPORT extern void wb_differential_wheels_disable_encoders(WbRobotContext *context);
CONTROLLER_EXPORT extern int wb_differential_wheels_get_encoders_sampling_period(WbRobotContext *context);
CONTROLLER_EXPORT extern double wb_differential_wheels_get_left_encoder(WbRobotContext *context);
CONTROLLER_EXPORT extern double wb_differential_wheels_get_right_encoder(WbRobotContext *context);
CONTROLLER_EXPORT extern void wb_differential_wheels_set_encoders(WbRobotContext *context, double left, double right);

#ifdef __cplusplus
//}
#endif

#endif /* WB_DIFFERENTIAL_WHEELS_H */
