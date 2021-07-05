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

// last modification: 19.12.01 by Yann
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "request.h"

#define scheduler_read_int32(d) (*((int *)(d)))

bool scheduler_init(WbRobotContext *context, const char *pipe, int robot_id);
void scheduler_cleanup(WbRobotContext *context);
WbRequest *scheduler_read_data(WbRobotContext *context);
void scheduler_send_request(WbRobotContext *context, WbRequest *);
bool scheduler_is_local(WbRobotContext *context);
int scheduler_get_pipe_handle(WbRobotContext *context);

#endif  // SCHEDULER_HH
