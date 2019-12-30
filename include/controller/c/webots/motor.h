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

#ifndef WB_MOTOR_H
#define WB_MOTOR_H

#define WB_USING_C_API
#include <webots/types.h>

#ifndef WB_MATLAB_LOADLIBRARY
#include <math.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

CONTROLLER_EXPORT void wb_motor_set_position(WbDeviceTag tag, double position);                  // rad or meters
CONTROLLER_EXPORT void wb_motor_set_acceleration(WbDeviceTag tag, double acceleration);          // rad/s^2 or m/s^2
CONTROLLER_EXPORT void wb_motor_set_velocity(WbDeviceTag tag, double velocity);                  // rad/s or m/s
CONTROLLER_EXPORT void wb_motor_set_force(WbDeviceTag tag, double force);                        // N
CONTROLLER_EXPORT void wb_motor_set_torque(WbDeviceTag tag, double torque);                      // N*m
CONTROLLER_EXPORT void wb_motor_set_available_force(WbDeviceTag tag, double force);              // N
CONTROLLER_EXPORT void wb_motor_set_available_torque(WbDeviceTag tag, double torque);            // N*m
CONTROLLER_EXPORT void wb_motor_set_control_pid(WbDeviceTag tag, double p, double i, double d);  // set the PID control parameters

CONTROLLER_EXPORT void wb_motor_enable_force_feedback(WbDeviceTag tag, int sampling_period);
CONTROLLER_EXPORT void wb_motor_disable_force_feedback(WbDeviceTag tag);
CONTROLLER_EXPORT int wb_motor_get_force_feedback_sampling_period(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_force_feedback(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_motor_enable_torque_feedback(WbDeviceTag tag, int sampling_period);
CONTROLLER_EXPORT void wb_motor_disable_torque_feedback(WbDeviceTag tag);
CONTROLLER_EXPORT int wb_motor_get_torque_feedback_sampling_period(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_torque_feedback(WbDeviceTag tag);

CONTROLLER_EXPORT WbJointType wb_motor_get_type(WbDeviceTag tag);

CONTROLLER_EXPORT double wb_motor_get_target_position(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_min_position(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_max_position(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_velocity(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_max_velocity(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_acceleration(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_available_force(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_max_force(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_available_torque(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_motor_get_max_torque(WbDeviceTag tag);

CONTROLLER_EXPORT WbDeviceTag wb_motor_get_brake(WbDeviceTag tag);
CONTROLLER_EXPORT WbDeviceTag wb_motor_get_position_sensor(WbDeviceTag tag);

#ifdef __cplusplus
}
#endif

#endif /* WB_MOTOR_H */
