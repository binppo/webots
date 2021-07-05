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

#ifndef WB_ROBOT_WWI_H
#define WB_ROBOT_WWI_H

#include <webots/types.h>

#ifdef __cplusplus
#include <cstring>
//extern "C" {
#else
#include <string.h>
#endif

CONTROLLER_EXPORT extern void wb_robot_wwi_send(WbRobotContext *context, const char *data, int size);
CONTROLLER_EXPORT extern const char *wb_robot_wwi_receive(WbRobotContext *context, int *size);
#define wb_robot_wwi_send_text(ctx, t) wb_robot_wwi_send(ctx, t, strlen(t) + 1)
#define wb_robot_wwi_receive_text(ctx) wb_robot_wwi_receive(ctx, NULL)

#ifdef __cplusplus
//}
#endif

#endif /* WB_ROBOT_WWI_H */
