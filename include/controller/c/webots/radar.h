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
/* Description:  Webots C programming interface for the Radar node                */
/**********************************************************************************/

#ifndef WB_RADAR_H
#define WB_RADAR_H

#define WB_USING_C_API
#include <webots/radar_target.h>
#include <webots/types.h>

#ifdef __cplusplus
extern "C" {
#endif

CONTROLLER_EXPORT void wb_radar_enable(WbDeviceTag tag, int sampling_period);
CONTROLLER_EXPORT void wb_radar_disable(WbDeviceTag tag);
CONTROLLER_EXPORT int wb_radar_get_sampling_period(WbDeviceTag tag);

CONTROLLER_EXPORT int wb_radar_get_number_of_targets(WbDeviceTag tag);
CONTROLLER_EXPORT const WbRadarTarget *wb_radar_get_targets(WbDeviceTag tag);

CONTROLLER_EXPORT double wb_radar_get_min_range(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_radar_get_max_range(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_radar_get_horizontal_fov(WbDeviceTag tag);
CONTROLLER_EXPORT double wb_radar_get_vertical_fov(WbDeviceTag tag);

#ifdef WB_MATLAB_LOADLIBRARY
// This function should be used only in the Matlab wrapper
CONTROLLER_EXPORT const WbRadarTarget *wb_radar_get_target(WbDeviceTag tag, int index);
#endif

#ifdef __cplusplus
}
#endif

#endif /* WB_RADAR_H */
