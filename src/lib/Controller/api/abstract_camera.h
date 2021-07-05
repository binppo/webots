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

#ifndef ABSTRACT_CAMERA_PRIVATE_H
#define ABSTRACT_CAMERA_PRIVATE_H

#include <webots/types.h>

#include <string>

#include "device_private.h"

#ifdef _WIN32
#include <windows.h>
#endif

struct AbstractCamera {
  bool enable;
  int sampling_period;
  unsigned int unique_id;  // camera id
  int width;
  int height;
  std::string shm_key;
  int shmid;
  double camnear;
  bool spherical;
  double fov;  // in degrees
  unsigned char *image;
  int mode;
  bool image_requested;
  double image_update_time;
  void *pdata;
#ifdef _WIN32
  HANDLE shm_file;
#endif
};

void wb_abstract_camera_cleanup(WbDeviceStruct *d);

void wb_abstract_camera_new(WbDeviceStruct *d, unsigned int id, int w, int h, double fov, double camnear, bool spherical);

void wb_abstract_camera_write_request(WbRobotContext *context, WbDeviceStruct *d, WbRequest *r);
bool wb_abstract_camera_handle_command(WbRobotContext *context, WbDeviceStruct *d, WbRequest *r, unsigned char command);

void abstract_camera_toggle_remote(WbDeviceStruct *d, WbRequest *r);

bool abstract_camera_request_image(WbRobotContext *context, AbstractCamera *ac, const char *functionName);
void wbr_abstract_camera_set_image(WbRobotContext *context, WbDeviceStruct *d, const unsigned char *image);
unsigned char *wbr_abstract_camera_get_image_buffer(WbRobotContext *context, WbDeviceStruct *d);

void wb_abstract_camera_enable(WbRobotContext *context, WbDeviceStruct *d, int sampling_period);
int wb_abstract_camera_get_sampling_period(WbRobotContext *context, WbDeviceStruct *d);
int wb_abstract_camera_get_height(WbRobotContext *context, WbDeviceStruct *d);
int wb_abstract_camera_get_width(WbRobotContext *context, WbDeviceStruct *d);
double wb_abstract_camera_get_fov(WbRobotContext *context, WbDeviceStruct *d);
double wb_abstract_camera_get_near(WbRobotContext *context, WbDeviceStruct *d);
void wb_abstract_camera_update_timestamp(WbRobotContext *context, WbDeviceStruct *d);

#endif  // ABSTRACT_CAMERA_PRIVATE_H
