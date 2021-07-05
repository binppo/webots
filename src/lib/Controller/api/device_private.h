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

#ifndef DEVICE_PRIVATE_H
#define DEVICE_PRIVATE_H

#include <webots/nodes.h>
#include <string>
#include "request.h"

#ifndef NAN
#define NAN (0.0 / 0.0)
#endif

struct WbDeviceStruct {
  WbNodeType node;
  std::string name;
  std::string model;
  void *pdata;
  void (*read_answer)(WbRobotContext *, WbDeviceStruct *, WbRequest *);
  void (*write_request)(WbRobotContext *, WbDeviceStruct *, WbRequest *);
  void (*cleanup)(WbRobotContext *, WbDeviceStruct *);
  void (*toggle_remote)(WbRobotContext *, WbDeviceStruct *, WbRequest *);
};

void wb_device_cleanup(WbRobotContext *context, WbDeviceStruct *);
void wb_device_init(WbDeviceStruct *);

#endif  // DEVICE_PRIVATE_H
