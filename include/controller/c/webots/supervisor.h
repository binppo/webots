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
/* Description:  Webots C programming interface for the Supervisor node           */
/**********************************************************************************/

#ifndef WB_SUPERVISOR_H
#define WB_SUPERVISOR_H

#define WB_USING_C_API
#include <webots/nodes.h>
#include <webots/types.h>

#include <string>

#ifdef __cplusplus
//extern "C" {
#endif

typedef enum {
  WB_NO_FIELD = 0x00,
  WB_SF_BOOL = 0x01,
  WB_SF_INT32,
  WB_SF_FLOAT,
  WB_SF_VEC2F,
  WB_SF_VEC3F,
  WB_SF_ROTATION,
  WB_SF_COLOR,
  WB_SF_STRING,
  WB_SF_NODE,
  WB_MF = 0x10,
  WB_MF_BOOL,
  WB_MF_INT32,
  WB_MF_FLOAT,
  WB_MF_VEC2F,
  WB_MF_VEC3F,
  WB_MF_ROTATION,
  WB_MF_COLOR,
  WB_MF_STRING,
  WB_MF_NODE
} WbFieldType;

typedef enum {
  WB_SUPERVISOR_SIMULATION_MODE_NONE = -1,
  WB_SUPERVISOR_SIMULATION_MODE_PAUSE = 0,
  WB_SUPERVISOR_SIMULATION_MODE_REAL_TIME,
  WB_SUPERVISOR_SIMULATION_MODE_RUN,
  WB_SUPERVISOR_SIMULATION_MODE_FAST
} WbSimulationMode;

struct CONTROLLER_EXPORT WbFieldData {
  bool sf_bool;
  int sf_int32;
  double sf_float;
  double sf_vec2f[2];
  double sf_vec3f[3];
  double sf_rotation[4];
  std::string sf_string;
  int sf_node_uid;  // 0 => NULL node
};

struct CONTROLLER_EXPORT WbFieldStruct {
  std::string name;
  WbFieldType type;  // WB_SF_* or WB_MT_* as defined in supervisor.h
  int count;         // used in MF fields only
  int node_unique_id;
  int id;  // attributed by Webots
  WbRobotContext *context;
  WbFieldData data;
  WbFieldRef next;
};

struct CONTROLLER_EXPORT WbFieldRequest {
  enum FIELD_REQUEST_TYPE type;
  int index;
  bool is_string;
  WbFieldData data;
  WbFieldStruct *field;
  WbFieldRequest *next;
};

struct CONTROLLER_EXPORT WbNodeStruct {
  int id;
  WbRobotContext *context;
  WbNodeType type;
  std::string model_name;
  std::string def_name;
  int parent_id;
  double *position;        // double[3]
  double *orientation;     // double[9]
  double *center_of_mass;  // double[3]
  int number_of_contact_points;
  double *contact_points;  // double[3 * number_of_contact_points]
  double contact_points_time_stamp;
  bool static_balance;
  double *solid_velocity;  // double[6] (linear[3] + angular[3])
  WbNodeRef next;
};

CONTROLLER_EXPORT extern void wb_supervisor_world_load(WbRobotContext *context, const char *filename);
CONTROLLER_EXPORT extern bool wb_supervisor_world_save(WbRobotContext *context, const char *filename);
CONTROLLER_EXPORT extern void wb_supervisor_world_reload(WbRobotContext *context);

CONTROLLER_EXPORT extern void wb_supervisor_simulation_quit(WbRobotContext *context, int status);
CONTROLLER_EXPORT extern void wb_supervisor_simulation_reset(WbRobotContext *context);
CONTROLLER_EXPORT extern void wb_supervisor_simulation_reset_physics(WbRobotContext *context);

CONTROLLER_EXPORT extern WbSimulationMode wb_supervisor_simulation_get_mode(WbRobotContext *context);
CONTROLLER_EXPORT extern void wb_supervisor_simulation_set_mode(WbRobotContext *context, WbSimulationMode mode);

CONTROLLER_EXPORT extern void wb_supervisor_set_label(WbRobotContext *context, int id, const char *text, double x, double y, double size, int color, double transparency,
                             const char *font);

CONTROLLER_EXPORT extern void wb_supervisor_export_image(WbRobotContext *context, const char *filename, int quality);

CONTROLLER_EXPORT extern void wb_supervisor_movie_start_recording(WbRobotContext *context, const char *filename, int width, int height, int codec, int quality, int acceleration,
                                         bool caption);
CONTROLLER_EXPORT extern void wb_supervisor_movie_stop_recording(WbRobotContext *context);
CONTROLLER_EXPORT extern bool wb_supervisor_movie_is_ready(WbRobotContext *context);
CONTROLLER_EXPORT extern bool wb_supervisor_movie_failed(WbRobotContext *context);

CONTROLLER_EXPORT extern bool wb_supervisor_animation_start_recording(WbRobotContext *context, const char *filename);
CONTROLLER_EXPORT extern bool wb_supervisor_animation_stop_recording(WbRobotContext *context);

CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_node_get_root(WbRobotContext *context);
CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_node_get_self(WbRobotContext *context);
CONTROLLER_EXPORT extern int wb_supervisor_node_get_id(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_node_get_from_id(WbRobotContext *context, int id);
CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_node_get_from_def(WbRobotContext *context, const char *def);
CONTROLLER_EXPORT extern bool wb_supervisor_node_get_defs(WbRobotContext *context, char *buf, int len);
CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_node_get_parent_node(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_node_get_selected(WbRobotContext *context);
CONTROLLER_EXPORT extern WbNodeType wb_supervisor_node_get_type(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern WbFieldRef wb_supervisor_node_get_field(WbRobotContext *context, WbNodeRef node, const char *field_name);
CONTROLLER_EXPORT extern void wb_supervisor_node_remove(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern bool wb_supervisor_node_is_valid(WbRobotContext *context, WbNodeRef node);

CONTROLLER_EXPORT extern const char *wb_supervisor_node_get_def(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern const char *wb_supervisor_node_get_type_name(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern const char *wb_supervisor_node_get_base_type_name(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern const double *wb_supervisor_node_get_center_of_mass(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern const double *wb_supervisor_node_get_contact_point(WbRobotContext *context, WbNodeRef node, int index);
CONTROLLER_EXPORT extern int wb_supervisor_node_get_number_of_contact_points(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern const double *wb_supervisor_node_get_orientation(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern const double *wb_supervisor_node_get_position(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern bool wb_supervisor_node_get_static_balance(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern const double *wb_supervisor_node_get_velocity(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern void wb_supervisor_node_set_velocity(WbRobotContext *context, WbNodeRef node, const double velocity[6]);
CONTROLLER_EXPORT extern void wb_supervisor_node_reset_physics(WbRobotContext *context, WbNodeRef node);
CONTROLLER_EXPORT extern void wb_supervisor_node_restart_controller(WbRobotContext *context, WbNodeRef node);

CONTROLLER_EXPORT extern void wb_supervisor_node_move_viewpoint(WbRobotContext *context, WbNodeRef node);

CONTROLLER_EXPORT extern void wb_supervisor_node_set_visibility(WbRobotContext *context, WbNodeRef node, WbNodeRef from, bool visible);

CONTROLLER_EXPORT extern WbFieldType wb_supervisor_field_get_type(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern const char *wb_supervisor_field_get_type_name(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern int wb_supervisor_field_get_count(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern int wb_supervisor_field_is_valid(WbRobotContext *context, WbFieldRef field);

CONTROLLER_EXPORT extern bool wb_supervisor_field_get_sf_bool(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern int wb_supervisor_field_get_sf_int32(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern double wb_supervisor_field_get_sf_float(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_sf_vec2f(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_sf_vec3f(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_sf_rotation(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_sf_color(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern const char *wb_supervisor_field_get_sf_string(WbRobotContext *context, WbFieldRef field);
CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_field_get_sf_node(WbRobotContext *context, WbFieldRef field);

CONTROLLER_EXPORT extern bool wb_supervisor_field_get_mf_bool(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern int wb_supervisor_field_get_mf_int32(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern double wb_supervisor_field_get_mf_float(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_mf_vec2f(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_mf_vec3f(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_mf_color(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern const double *wb_supervisor_field_get_mf_rotation(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern const char *wb_supervisor_field_get_mf_string(WbRobotContext *context, WbFieldRef field, int index);
CONTROLLER_EXPORT extern WbNodeRef wb_supervisor_field_get_mf_node(WbRobotContext *context, WbFieldRef field, int index);

CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_bool(WbRobotContext *context, WbFieldRef field, bool value);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_int32(WbRobotContext *context, WbFieldRef field, int value);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_float(WbRobotContext *context, WbFieldRef field, double value);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_vec2f(WbRobotContext *context, WbFieldRef field, const double values[2]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_vec3f(WbRobotContext *context, WbFieldRef field, const double values[3]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_rotation(WbRobotContext *context, WbFieldRef field, const double values[4]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_color(WbRobotContext *context, WbFieldRef field, const double values[3]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_sf_string(WbRobotContext *context, WbFieldRef field, const char *value);

CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_bool(WbRobotContext *context, WbFieldRef field, int index, bool value);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_int32(WbRobotContext *context, WbFieldRef field, int index, int value);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_float(WbRobotContext *context, WbFieldRef field, int index, double value);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_vec2f(WbRobotContext *context, WbFieldRef field, int index, const double values[2]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_vec3f(WbRobotContext *context, WbFieldRef field, int index, const double values[3]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_rotation(WbRobotContext *context, WbFieldRef field, int index, const double values[4]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_color(WbRobotContext *context, WbFieldRef field, int index, const double values[3]);
CONTROLLER_EXPORT extern void wb_supervisor_field_set_mf_string(WbRobotContext *context, WbFieldRef field, int index, const char *value);

CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_bool(WbRobotContext *context, WbFieldRef field, int index, bool value);
CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_int32(WbRobotContext *context, WbFieldRef field, int index, int value);
CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_float(WbRobotContext *context, WbFieldRef field, int index, double value);
CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_vec2f(WbRobotContext *context, WbFieldRef field, int index, const double values[2]);
CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_vec3f(WbRobotContext *context, WbFieldRef field, int index, const double values[3]);
CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_rotation(WbRobotContext *context, WbFieldRef field, int index, const double values[4]);
CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_color(WbRobotContext *context, WbFieldRef field, int index, const double values[3]);
CONTROLLER_EXPORT extern void wb_supervisor_field_insert_mf_string(WbRobotContext *context, WbFieldRef field, int index, const char *value);

CONTROLLER_EXPORT extern void wb_supervisor_field_remove_mf(WbRobotContext *context, WbFieldRef field, int index);

CONTROLLER_EXPORT extern void wb_supervisor_field_import_mf_node(WbRobotContext *context, WbFieldRef field, int position, const char *filename);
CONTROLLER_EXPORT extern void wb_supervisor_field_import_mf_node_from_string(WbRobotContext *context, WbFieldRef field, int position, const char *node_string);

CONTROLLER_EXPORT extern bool wb_supervisor_virtual_reality_headset_is_used(WbRobotContext *context);
CONTROLLER_EXPORT extern const double *wb_supervisor_virtual_reality_headset_get_position(WbRobotContext *context);
CONTROLLER_EXPORT extern const double *wb_supervisor_virtual_reality_headset_get_orientation(WbRobotContext *context);

// Deprecated functions
// deprecated since Webots R2018b
CONTROLLER_EXPORT extern void wb_supervisor_simulation_revert(WbRobotContext *context) WB_DEPRECATED;               // please use wb_supervisor_world_reload() instead
CONTROLLER_EXPORT extern void wb_supervisor_load_world(WbRobotContext *context, const char *filename) WB_DEPRECATED;  // please use wb_supervisor_world_load() instead
CONTROLLER_EXPORT extern bool wb_supervisor_save_world(WbRobotContext *context, const char *filename) WB_DEPRECATED;  // please use wb_supervisor_world_save() instead

// deprecated since Webots 8.6.0, plesae use wb_supervisor_field_remove_mf_item() instead
CONTROLLER_EXPORT extern void wb_supervisor_field_remove_mf_node(WbRobotContext *context, WbFieldRef field, int position) WB_DEPRECATED;

// deprecated since Webots 8.0.0, plesae use wb_supervisor_simulation_reset_physics() instead
CONTROLLER_EXPORT extern void wb_supervisor_simulation_physics_reset(WbRobotContext *context) WB_DEPRECATED;

// deprecated since Webots 8.4.0 please use wb_supervisor_movie_is_ready and wb_supervisor_movie_failed
#define WB_SUPERVISOR_MOVIE_READY 0
#define WB_SUPERVISOR_MOVIE_RECORDING 1
#define WB_SUPERVISOR_MOVIE_SAVING 2
#define WB_SUPERVISOR_MOVIE_WRITE_ERROR 3
#define WB_SUPERVISOR_MOVIE_ENCODING_ERROR 4
#define WB_SUPERVISOR_MOVIE_SIMULATION_ERROR 5
CONTROLLER_EXPORT extern int wb_supervisor_movie_get_status(WbRobotContext *context);

// deprecated since webots 8.3.0: please use the wb_supervisor_movie_*() functions instead
CONTROLLER_EXPORT extern void wb_supervisor_start_movie(WbRobotContext *context, const char *file, int width, int height, int codec, int quality, int acceleration,
                               bool caption) WB_DEPRECATED;
CONTROLLER_EXPORT extern void wb_supervisor_stop_movie(WbRobotContext *context) WB_DEPRECATED;
CONTROLLER_EXPORT extern int wb_supervisor_get_movie_status(WbRobotContext *context) WB_DEPRECATED;

#ifdef __cplusplus
//}
#endif

#endif /* SUPERVISOR_H */
