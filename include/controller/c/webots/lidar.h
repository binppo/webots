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
/* Description:  Webots C programming interface for the Lidar node                */
/**********************************************************************************/

#ifndef WB_LIDAR_H
#define WB_LIDAR_H

#define WB_USING_C_API
#include <webots/lidar_point.h>
#include <webots/types.h>

#ifdef __cplusplus
//extern "C" {
#endif

CONTROLLER_EXPORT extern void wb_lidar_enable(WbRobotContext *context, WbDeviceTag tag, int sampling_period);
CONTROLLER_EXPORT extern void wb_lidar_enable_point_cloud(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern void wb_lidar_disable(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern void wb_lidar_disable_point_cloud(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern int wb_lidar_get_sampling_period(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern bool wb_lidar_is_point_cloud_enabled(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern const float *wb_lidar_get_range_image(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern const float *wb_lidar_get_layer_range_image(WbRobotContext *context, WbDeviceTag tag, int layer);

CONTROLLER_EXPORT extern const WbLidarPoint *wb_lidar_get_point_cloud(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern const WbLidarPoint *wb_lidar_get_layer_point_cloud(WbRobotContext *context, WbDeviceTag tag, int layer);
CONTROLLER_EXPORT extern int wb_lidar_get_number_of_points(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern int wb_lidar_get_horizontal_resolution(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern int wb_lidar_get_number_of_layers(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern double wb_lidar_get_min_frequency(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern double wb_lidar_get_max_frequency(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern double wb_lidar_get_frequency(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern void wb_lidar_set_frequency(WbRobotContext *context, WbDeviceTag tag, double frequency);
CONTROLLER_EXPORT extern double wb_lidar_get_fov(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern double wb_lidar_get_vertical_fov(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern double wb_lidar_get_min_range(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern double wb_lidar_get_max_range(WbRobotContext *context, WbDeviceTag tag);

#ifdef WB_MATLAB_LOADLIBRARY
// This function should be used only in the Matlab wrapper
CONTROLLER_EXPORT extern const WbLidarPoint *wb_lidar_get_point(WbRobotContext *context, WbDeviceTag tag, int index);
#endif

#ifdef __cplusplus
//}
#endif

#endif /* WB_LIDAR_H */
