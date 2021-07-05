#include <webots/device.h>
#include <webots/robot.h>
#include <webots/robot_wwi.h>
#include <webots/utils/default_robot_window.h>

#include <webots/accelerometer.h>
#include <webots/camera.h>
#include <webots/compass.h>
#include <webots/differential_wheels.h>
#include <webots/distance_sensor.h>
#include <webots/gps.h>
#include <webots/gyro.h>
#include <webots/inertial_unit.h>
#include <webots/lidar.h>
#include <webots/light_sensor.h>
#include <webots/motor.h>
#include <webots/position_sensor.h>
#include <webots/radar.h>
#include <webots/range_finder.h>
#include <webots/touch_sensor.h>

#include <base/string_util.h>
#include <base/string_number_conversions.h>
#include <base/strings/string_tokenizer.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//#include "string_utils.h"

static int time_step = 0;
static double max_speed = 0.0;

static void enable_device(WbRobotContext *context, WbDeviceTag tag, bool enable) {
  WbNodeType type = wb_device_get_node_type(context, tag);
  int enableRate = enable ? time_step : 0;
  switch (type) {
    case WB_NODE_ACCELEROMETER:
      wb_accelerometer_enable(context, tag, enableRate);
      break;
    case WB_NODE_CAMERA:
      wb_camera_enable(context, tag, enableRate);
      break;
    case WB_NODE_COMPASS:
      wb_compass_enable(context, tag, enableRate);
      break;
    case WB_NODE_DISTANCE_SENSOR:
      wb_distance_sensor_enable(context, tag, enableRate);
      break;
    case WB_NODE_GPS:
      wb_gps_enable(context, tag, enableRate);
      break;
    case WB_NODE_GYRO:
      wb_gyro_enable(context, tag, enableRate);
      break;
    case WB_NODE_INERTIAL_UNIT:
      wb_inertial_unit_enable(context, tag, enableRate);
      break;
    case WB_NODE_LIDAR:
      wb_lidar_enable(context, tag, enableRate);
      break;
    case WB_NODE_LIGHT_SENSOR:
      wb_light_sensor_enable(context, tag, enableRate);
      break;
    case WB_NODE_POSITION_SENSOR:
      wb_position_sensor_enable(context, tag, enableRate);
      break;
    case WB_NODE_RADAR:
      wb_radar_enable(context, tag, enableRate);
      break;
    case WB_NODE_RANGE_FINDER:
      wb_range_finder_enable(context, tag, enableRate);
      break;
    case WB_NODE_TOUCH_SENSOR:
      wb_touch_sensor_enable(context, tag, enableRate);
      break;
    default:
      assert(0);
  }
}

// javascript -> C protocol description:
//   [deviceName:commandTag[=commadState][,]]*
// example:
//   "e-puck:forward,ds0:enable,myMotor0:value=1.2"
static void apply_command(WbRobotContext *context, const char *command) {
  WbDeviceTag tag = 0;
  bool robot = false;

  base::CStringTokenizer t(command, command + strlen(command), ":");
  while (t.GetNext()) {
    if (tag == 0 && !robot) {  // first token = device or robot name
      std::string name(t.token());
      ReplaceSubstringsAfterOffset(&name, 0, "\\:", ":");
      ReplaceSubstringsAfterOffset(&name, 0, "\\,", ",");

      if (strcmp(name.c_str(), wb_robot_get_name(context)) == 0)
        robot = true;
      else
        tag = wb_robot_get_device(context, name.c_str());
    } else if (strcmp(t.token().c_str(), "enable") == 0)
      enable_device(context, tag, true);
    else if (strcmp(t.token().c_str(), "disable") == 0)
      enable_device(context, tag, false);
    else if (strcmp(t.token().c_str(), "stop") == 0 && robot)
      wb_differential_wheels_set_speed(context, 0.0, 0.0);
    else if (strcmp(t.token().c_str(), "forward") == 0 && robot)
      wb_differential_wheels_set_speed(context, max_speed, max_speed);
    else if (strcmp(t.token().c_str(), "backward") == 0 && robot)
      wb_differential_wheels_set_speed(context, -max_speed, -max_speed);
    else if (strcmp(t.token().c_str(), "left") == 0 && robot)
      wb_differential_wheels_set_speed(context, -0.5 * max_speed, 0.5 * max_speed);
    else if (strcmp(t.token().c_str(), "left_forward") == 0 && robot)
      wb_differential_wheels_set_speed(context, 0.5 * max_speed, max_speed);
    else if (strcmp(t.token().c_str(), "left_backward") == 0 && robot)
      wb_differential_wheels_set_speed(context, -0.5 * max_speed, -max_speed);
    else if (strcmp(t.token().c_str(), "right") == 0 && robot)
      wb_differential_wheels_set_speed(context, 0.5 * max_speed, -0.5 * max_speed);
    else if (strcmp(t.token().c_str(), "right_forward") == 0 && robot)
      wb_differential_wheels_set_speed(context, max_speed, 0.5 * max_speed);
    else if (strcmp(t.token().c_str(), "right_backward") == 0 && robot)
      wb_differential_wheels_set_speed(context, -max_speed, -0.5 * max_speed);
    else if (strncmp(t.token().c_str(), "position=", 9) == 0 && tag > 0) {
      double position = 0.0;
      base::StringToDouble(t.token(), &position);
      wb_motor_set_position(context, tag, position);
    } else if (strcmp(t.token().c_str(), "pointCloudEnable") == 0)
      wb_lidar_enable_point_cloud(context, tag);
    else if (strcmp(t.token().c_str(), "pointCloudDisable") == 0)
      wb_lidar_disable_point_cloud(context, tag);
    else
      assert(0);  // protocol issue
  }
}

static void apply_commands(WbRobotContext *context, const char *commands) {
  base::CStringTokenizer t(commands, commands + strlen(commands), ",");
  while (t.GetNext())
    apply_command(context, t.token().c_str());
}

void wb_robot_window_init(WbRobotContext *context) {
  time_step = (int)wb_robot_get_basic_time_step(context);
  WbNodeType robot_type = wb_device_get_node_type(context, 0);
  if (robot_type == WB_NODE_DIFFERENTIAL_WHEELS)
    max_speed = wb_differential_wheels_get_max_speed(context) / wb_differential_wheels_get_speed_unit(context);
}

void wb_robot_window_step(WbRobotContext *context, int time_step) {
  static bool configured = false;
  const char *message = wb_robot_wwi_receive_text(context);
  if (message) {
    if (strncmp(message, "configure", 9) == 0) {
      int max_image_height = -1;
      int max_image_width = -1;
      if (sscanf(message, "configure { \"imageMaxWidth\": %d, \"imageMaxHeight\": %d }", &max_image_width, &max_image_height) !=
          2) {
        fprintf(stderr, "Wrong 'configure' message received from the robot window.\n");
        assert(0);
        return;
      }
      wbu_default_robot_window_set_images_max_size(context, max_image_width, max_image_height);
      wbu_default_robot_window_configure(context);
      configured = true;
    } else
      apply_commands(context, message);
  }

  if (!configured)
    return;

  wbu_default_robot_window_update(context);
}

void wb_robot_window_cleanup() {
}
