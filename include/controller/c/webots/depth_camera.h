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
/* Description:  Webots C programming interface for the DepthCamera node                */
/**********************************************************************************/

#ifndef WB_DEPTH_CAMERA_H
#define WB_DEPTH_CAMERA_H

#define WB_USING_C_API
#include <webots/types.h>

#ifdef __cplusplus
extern "C" {
#endif

CONTROLLER_EXPORT void wb_depthcamera_enable(WbDeviceTag tag, int sampling_period);
CONTROLLER_EXPORT void wb_depthcamera_enable_point_cloud(WbDeviceTag tag);
CONTROLLER_EXPORT void wb_depthcamera_disable(WbDeviceTag tag);
CONTROLLER_EXPORT void wb_depthcamera_disable_point_cloud(WbDeviceTag tag);
CONTROLLER_EXPORT bool wb_depthcamera_is_point_cloud_enabled(WbDeviceTag tag);

CONTROLLER_EXPORT const float *wb_depthcamera_get_range_image(WbDeviceTag tag);

//CONTROLLER_EXPORT const WbDepthPoint *wb_depth_get_point_cloud(WbDeviceTag tag);
//CONTROLLER_EXPORT const WbDepthPoint wb_depth_get_point(WbDeviceTag tag, int index);
CONTROLLER_EXPORT int wb_depthcamera_get_number_of_points(WbDeviceTag tag);

CONTROLLER_EXPORT int wb_depthcamera_get_horizontal_resolution(WbDeviceTag tag);
CONTROLLER_EXPORT int wb_depthcamera_get_vertical_resolution(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_depthcamera_get_fov(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_depthcamera_get_vertical_fov(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_depthcamera_get_horizontal_fov(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_depthcamera_get_min_range(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_depthcamera_get_max_range(WbDeviceTag tag);

#ifdef __cplusplus
}
#endif

#endif /* WB_DEPTH_CAMERA_H */
