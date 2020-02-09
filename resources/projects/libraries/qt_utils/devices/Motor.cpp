#include "Motor.hpp"

#include <webots/motor.h>
#include <webots/position_sensor.h>

using namespace webotsQtUtils;

Motor::Motor(WbDeviceTag tag) : Device(tag) {
}

void Motor::enable(bool enable) {
  WbDeviceTag sensor = wb_motor_get_position_sensor(mTag);
  if(sensor>=0)
  {
    if(enable)
      wb_position_sensor_enable(sensor, wb_position_sensor_get_sampling_period(sensor));
    else
      wb_position_sensor_disable(sensor);
  }
  else
  {
    if (wb_motor_get_type(mTag) == WB_ROTATIONAL) {
      if (enable)
        wb_motor_enable_torque_feedback(mTag, static_cast<int>(wb_robot_get_basic_time_step()));
      else
        wb_motor_disable_torque_feedback(mTag);
    } else {
      if (enable)
        wb_motor_enable_force_feedback(mTag, static_cast<int>(wb_robot_get_basic_time_step()));
      else
        wb_motor_disable_force_feedback(mTag);
    }
  }
}

bool Motor::isEnabled() const {
  if (wb_motor_get_type(mTag) == WB_ROTATIONAL)
    return wb_motor_get_torque_feedback_sampling_period(mTag) > 0;
  else
    return wb_motor_get_force_feedback_sampling_period(mTag) > 0;
}

double Motor::minPosition() const {
  return wb_motor_get_min_position(mTag);
}

double Motor::maxPosition() const {
  return wb_motor_get_max_position(mTag);
}

double Motor::position() const {
  WbDeviceTag sensor = wb_motor_get_position_sensor(mTag);
  if(sensor>=0)
  {
	  return wb_position_sensor_get_value(sensor);
  }
  else
  {
    if (wb_motor_get_type(mTag) == WB_ROTATIONAL)
      return wb_motor_get_torque_feedback(mTag);
    else
      return wb_motor_get_force_feedback(mTag);
  }
}

double Motor::targetPosition() const {
  return wb_motor_get_target_position(mTag);
}

void Motor::setPosition(double position) {
  wb_motor_set_position(mTag, position);
}
