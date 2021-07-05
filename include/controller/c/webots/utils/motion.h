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
/* Description:  Webots C programming interface for Motion file playback          */
/**********************************************************************************/

#ifndef WBU_MOTION_H
#define WBU_MOTION_H

#include <webots/types.h>

#include <string>
#include <vector>

#ifdef __cplusplus
//extern "C" {
#endif

struct WbMotionStruct {
  std::string filename;      // file name
  std::vector<std::string> joint_names;  // array of n_joints joint names
  std::vector<WbDeviceTag> tags;   // array of n_joints device tags
  std::vector<WbNodeType> types;   // array of n_joints device types
  std::vector<int> times;          // array of n_poses poses
  std::vector<double*> poses;        // two-dimensional [n_poses][n_joints] array
  int elapsed;         // elapsed time when playing this motion file
  bool playing;        // is currenly playing
  bool reverse;        // playing forward of backwards ?
  bool loop;           // loop when reaching the end (or begining) ?
  WbMotionRef next;    // next struct in list
};

CONTROLLER_EXPORT extern WbMotionRef wbu_motion_new(WbRobotContext *context, const char *filename);
CONTROLLER_EXPORT extern void wbu_motion_delete(WbMotionRef motion);
CONTROLLER_EXPORT extern WbMotionRef wbu_motion_get_head();
CONTROLLER_EXPORT extern bool wbu_motion_is_valid(WbMotionRef motion);

CONTROLLER_EXPORT extern void wbu_motion_play(WbMotionRef motion);
CONTROLLER_EXPORT extern void wbu_motion_stop(WbMotionRef motion);
CONTROLLER_EXPORT extern void wbu_motion_set_loop(WbMotionRef motion, bool loop);
CONTROLLER_EXPORT extern void wbu_motion_set_reverse(WbMotionRef motion, bool reverse);

CONTROLLER_EXPORT extern bool wbu_motion_is_over(WbMotionRef motion);
CONTROLLER_EXPORT extern int wbu_motion_get_duration(WbMotionRef motion);
CONTROLLER_EXPORT extern int wbu_motion_get_time(WbMotionRef motion);
CONTROLLER_EXPORT extern void wbu_motion_set_time(WbMotionRef motion, int time);

#ifdef __cplusplus
//}
#endif

#endif /* WBU_MOTION_H */
