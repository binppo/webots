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
/* Description:  Webots C programming interface for the Robot node                */
/**********************************************************************************/

#ifndef WB_ROBOT_H
#define WB_ROBOT_H

#define WB_USING_C_API
#include <webots/types.h>
#include <webots/supervisor.h>

#ifdef __CYGWIN__
#include <stdio.h>
#endif

#include <limits>
#include <string>
#include <vector>

#if defined(__VISUALC__) || defined(_MSC_VER)
#include "stdio.h"
#endif

#include "nodes.h"

#ifdef KROS_COMPILATION
#define main() _kros_main()
#endif

typedef enum { WB_MODE_SIMULATION = 0, WB_MODE_CROSS_COMPILATION, WB_MODE_REMOTE_CONTROL } WbRobotMode;

typedef enum {
  WB_EVENT_QUIT = -1,
  WB_EVENT_NO_EVENT = 0,
  WB_EVENT_MOUSE_CLICK = 1,
  WB_EVENT_MOUSE_MOVE = 2,
  WB_EVENT_KEYBOARD = 4,
  WB_EVENT_JOYSTICK_BUTTON = 8,
  WB_EVENT_JOYSTICK_AXIS = 16,
  WB_EVENT_JOYSTICK_POV = 32
} WbUserInputEvent;

// cart function headers
#ifdef __cplusplus
//extern "C" {
#endif

struct UpdateElement;
struct GPipe;
struct Label;

struct CONTROLLER_EXPORT WbRobotStruct {
  std::vector<WbDeviceStruct*> device;  // array of devices
  double battery_value = std::numeric_limits<double>::quiet_NaN();
  void (*real_robot_cleanup)(void) = NULL;
  int battery_sampling_period = 0;
  int n_device;  // number of devices, including the robot itself
  bool is_supervisor = false;
  unsigned char synchronization = 0;
  WbRobotMode mode = WB_MODE_SIMULATION;
  unsigned char configure = 0;  // 1 if just configured: reset need to be called
  unsigned char client_exit = false;
  unsigned char webots_exit = 0;  // WEBOTS_EXIT_FALSE, WEBOTS_EXIT_NOW or WEBOTS_EXIT_LATER
  double basic_time_step;
  char console_stream;
  std::string console_text;
  std::string project_path;
  std::string world_path;
  std::string model;
  std::string window_filename;
  std::string remote_control_filename;
  std::string controller_name;
  std::string arguments;
  std::string custom_data;
  bool is_waiting_for_user_input_event = false;
  WbUserInputEvent user_input_event_type;
  int user_input_event_timeout;
  bool dataNeedToWriteRequest = false;
  bool show_window;
  bool has_html_robot_window;
  bool update_window;
  bool toggle_remote_first_step;  // true in the first step after a switch between remote/simulation
  bool send_remote_mode_off;      // tell to send the REMOTE_OFF message in the next robot_write_request()
  int pin = -1;
  int wwi_message_to_send_size;
  const char *wwi_message_to_send;
  int wwi_message_received_size;
  char *wwi_message_received;
  WbSimulationMode simulation_mode;  // WB_SUPERVISOR_SIMULATION_MODE_RUN, etc.
};

struct CONTROLLER_EXPORT WbRobotContext {
public:
  WbRobotContext()
  : robot_init_was_done(false)
  , robot(NULL)
  , robot_step_mutex(NULL)
  , simulation_time(0.0)
  , current_step_duration(0)
  , robot_id(0)
  , field_list(NULL)
  , field_requests_list_head(NULL)
  , field_requests_list_tail(NULL)
  , field_requests_garbage_list(NULL)
  , sent_field_get_request(NULL)
  , node_list(NULL)
  , export_image_quality(0)
  , simulation_quit(false)
  , simulation_quit_status(0)
  , simulation_reset(false)
  , world_reload(false)
  , simulation_reset_physics(false)
  , simulation_change_mode(false)
  , imported_nodes_number(-1)
  , movie_stop(false)
  , movie_status(WB_SUPERVISOR_MOVIE_READY)
  , movie_quality(0)
  , movie_codec(0)
  , movie_width(0)
  , movie_height(0)
  , movie_acceleration(1)
  , movie_caption(false)
  , animation_stop(false)
  , animation_start_status(true)
  , animation_stop_status(true)
  , save_status(true)
  , save_request(false)
  , node_id(-1)
  , node_to_remove(NULL)
  , node_get_selected(false)
  , selected_node_id(-1)
  , node_ref(0)
  , root_ref(NULL)
  , self_node_ref(NULL)
  , position_node_ref(NULL)
  , orientation_node_ref(NULL)
  , center_of_mass_node_ref(NULL)
  , contact_points_node_ref(NULL)
  , static_balance_node_ref(NULL)
  , reset_physics_node_ref(NULL)
  , restart_controller_node_ref(NULL)
  , node_visible(true)
  , move_viewpoint_node_ref(NULL)
  , set_visibility_node_ref(NULL)
  , set_visibility_from_node_ref(NULL)
  , get_velocity_node_ref(NULL)
  , set_velocity_node_ref(NULL)
  , solid_velocity(NULL)
  , virtual_reality_headset_is_used_request(false)
  , virtual_reality_headset_is_used(false)
  , virtual_reality_headset_position_request(false)
  , virtual_reality_headset_position(NULL)
  , virtual_reality_headset_orientation_request(false)
  , virtual_reality_headset_orientation(NULL)
  , robot_window_images_max_width(-1)
  , robot_window_images_max_height(-1)
  , scheduler_data_size(0)
  , scheduler_actual_step(0)
  , scheduler_data(NULL)
  , scheduler_pipe(NULL)
  , supervisor_label(NULL)
  {}

  bool robot_init_was_done;
  WbRobotStruct *robot;
  WbMutexRef robot_step_mutex;
  double simulation_time;
  unsigned int current_step_duration;
  int robot_id;

  WbFieldStruct *field_list;
  WbFieldRequest *field_requests_list_head;
  WbFieldRequest *field_requests_list_tail;
  WbFieldRequest *field_requests_garbage_list;
  WbFieldRequest *sent_field_get_request;

  WbNodeStruct *node_list;

  std::string export_image_filename;
  int export_image_quality;
  bool simulation_quit;
  int simulation_quit_status;
  bool simulation_reset;
  bool world_reload;
  bool simulation_reset_physics;
  bool simulation_change_mode;
  int imported_nodes_number;
  std::string world_to_load;
  char movie_stop;
  char movie_status;
  std::string movie_filename;
  int movie_quality;
  int movie_codec;
  int movie_width;
  int movie_height;
  int movie_acceleration;
  int movie_caption;
  char animation_stop;
  std::string animation_filename;
  bool animation_start_status;
  bool animation_stop_status;
  bool save_status;
  bool save_request;
  std::string save_filename;
  int node_id;
  WbNodeRef node_to_remove;
  std::string node_def_name;
  std::string requested_field_name;
  bool node_get_selected;
  int selected_node_id;
  int node_ref;
  WbNodeRef root_ref;
  WbNodeRef self_node_ref;
  WbNodeRef position_node_ref;
  WbNodeRef orientation_node_ref;
  WbNodeRef center_of_mass_node_ref;
  WbNodeRef contact_points_node_ref;
  WbNodeRef static_balance_node_ref;
  WbNodeRef reset_physics_node_ref;
  WbNodeRef restart_controller_node_ref;
  bool node_visible;
  WbNodeRef move_viewpoint_node_ref;
  WbNodeRef set_visibility_node_ref;
  WbNodeRef set_visibility_from_node_ref;
  WbNodeRef get_velocity_node_ref;
  WbNodeRef set_velocity_node_ref;
  const double *solid_velocity;
  bool virtual_reality_headset_is_used_request;
  bool virtual_reality_headset_is_used;
  bool virtual_reality_headset_position_request;
  double *virtual_reality_headset_position;
  bool virtual_reality_headset_orientation_request;
  double *virtual_reality_headset_orientation;

  std::string robot_window_buffer;
  int robot_window_images_max_width;
  int robot_window_images_max_height;
  std::vector<UpdateElement*> robot_window_update_elements;

  unsigned int scheduler_data_size;
  unsigned int scheduler_actual_step;
  char *scheduler_data;
  GPipe *scheduler_pipe;
  Label *supervisor_label;
};

/* In the visual studio case, the buffer size of the standard output and
 * the standard error cannot be modified from a dll
 */
CONTROLLER_EXPORT extern WbRobotContext *wb_robot_init(int id);

CONTROLLER_EXPORT extern int wb_robot_step(WbRobotContext *context, int duration);  // milliseconds

#ifdef __CYGWIN__  // In that case, we need to flush explicitly the stdout/stdin streams otherwise they are buffered
// We cannot call fflush from the libController as libController is compiled with gcc8 and won't flush the stdout/stderr
// of a gcc7 (cygwin) compiled binary. Therefore, we need to perform the fflush in a gcc7 compiled code, e.g., in a macro here.
#define wb_robot_step(context, d) (fflush(NULL) ? wb_robot_step(context, d) : wb_robot_step(d))
#endif

CONTROLLER_EXPORT extern WbUserInputEvent wb_robot_wait_for_user_input_event(WbRobotContext *context, WbUserInputEvent event_type, int timeout);  // milliseconds
CONTROLLER_EXPORT extern void wb_robot_cleanup(WbRobotContext *context);
CONTROLLER_EXPORT extern WbRobotContext *wb_robot_get_context(int);
CONTROLLER_EXPORT extern double wb_robot_get_time(WbRobotContext *context);
CONTROLLER_EXPORT extern const char *wb_robot_get_name(WbRobotContext *context);
CONTROLLER_EXPORT extern const char *wb_robot_get_model(WbRobotContext *context);
CONTROLLER_EXPORT extern const char *wb_robot_get_custom_data(WbRobotContext *context);
CONTROLLER_EXPORT extern void wb_robot_set_custom_data(WbRobotContext *context, const char *data);
CONTROLLER_EXPORT extern WbRobotMode wb_robot_get_mode(WbRobotContext *context);
CONTROLLER_EXPORT extern void wb_robot_set_mode(WbRobotContext *context, WbRobotMode mode, void *args);
CONTROLLER_EXPORT extern bool wb_robot_get_synchronization(WbRobotContext *context);
CONTROLLER_EXPORT extern bool wb_robot_get_supervisor(WbRobotContext *context);
CONTROLLER_EXPORT extern const char *wb_robot_get_project_path(WbRobotContext *context);
CONTROLLER_EXPORT extern const char *wb_robot_get_world_path(WbRobotContext *context);
CONTROLLER_EXPORT extern double wb_robot_get_basic_time_step(WbRobotContext *context);
CONTROLLER_EXPORT extern WbDeviceTag wb_robot_get_device(WbRobotContext *context, const char *name);

// Controller API
CONTROLLER_EXPORT extern const char *wb_robot_get_controller_name(WbRobotContext *context);
CONTROLLER_EXPORT extern const char *wb_robot_get_controller_arguments(WbRobotContext *context);

// Introspection API
CONTROLLER_EXPORT extern int wb_robot_get_number_of_devices(WbRobotContext *context);
CONTROLLER_EXPORT extern WbDeviceTag wb_robot_get_device_by_index(WbRobotContext *context, int index);
CONTROLLER_EXPORT extern WbNodeType wb_robot_get_type(WbRobotContext *context);

// robot battery API
CONTROLLER_EXPORT extern void wb_robot_battery_sensor_enable(WbRobotContext *context, int sampling_period);
CONTROLLER_EXPORT extern void wb_robot_battery_sensor_disable(WbRobotContext *context);
CONTROLLER_EXPORT extern int wb_robot_battery_sensor_get_sampling_period(WbRobotContext *context);
CONTROLLER_EXPORT extern double wb_robot_battery_sensor_get_value(WbRobotContext *context);

// robot multi-thread API
#ifndef WB_MATLAB_LOADLIBRARY
CONTROLLER_EXPORT extern void wb_robot_task_new(void (*task)(void *), void *param);  // create a task
CONTROLLER_EXPORT extern WbMutexRef wb_robot_mutex_new();
CONTROLLER_EXPORT extern void wb_robot_mutex_lock(WbMutexRef);
CONTROLLER_EXPORT extern void wb_robot_mutex_unlock(WbMutexRef);
CONTROLLER_EXPORT extern void wb_robot_mutex_delete(WbMutexRef);
#endif

// Motion editor specfic function : Please don't use this function outside qt_utils
// This function doesn't work if the robot window has not been shown at lease once
CONTROLLER_EXPORT extern void wb_robot_pin_to_static_environment(WbRobotContext *context, bool pin);

// Deprecated functions
// deprecated since Webots 2018a, please use wb_robot_get_custom_data and
// wb_robot_set_custom_data instead
CONTROLLER_EXPORT extern const char *wb_robot_get_data(WbRobotContext *context) WB_DEPRECATED;
CONTROLLER_EXPORT extern void wb_robot_set_data(WbRobotContext *context, const char *data) WB_DEPRECATED;

#ifdef __cplusplus
//}
#endif

#endif /* WB_ROBOT_H */
