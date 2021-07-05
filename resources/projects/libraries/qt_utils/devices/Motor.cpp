#include "Motor.hpp"

#include <webots/motor.h>
#include <webots/position_sensor.h>

using namespace webotsQtUtils;

Motor::Motor(WbRobotContext *ctx, WbDeviceTag tag) : Device(ctx, tag) {
}

void Motor::enable(bool enable) {
  WbDeviceTag sensor = wb_motor_get_position_sensor(mContext, mTag);
  if(sensor>=0)
  {
    if(enable)
      wb_position_sensor_enable(mContext, sensor, wb_position_sensor_get_sampling_period(mContext, sensor));
    else
      wb_position_sensor_disable(mContext, sensor);
  }
  else
  {
    if (wb_motor_get_type(mContext, mTag) == WB_ROTATIONAL) {
      if (enable)
        wb_motor_enable_torque_feedback(mContext, mTag, static_cast<int>(wb_robot_get_basic_time_step(mContext)));
      else
        wb_motor_disable_torque_feedback(mContext, mTag);
    } else {
      if (enable)
        wb_motor_enable_force_feedback(mContext, mTag, static_cast<int>(wb_robot_get_basic_time_step(mContext)));
      else
        wb_motor_disable_force_feedback(mContext, mTag);
    }
  }
}

bool Motor::isEnabled() const {
  if (wb_motor_get_type(mContext, mTag) == WB_ROTATIONAL)
    return wb_motor_get_torque_feedback_sampling_period(mContext, mTag) > 0;
  else
    return wb_motor_get_force_feedback_sampling_period(mContext, mTag) > 0;
}

double Motor::minPosition() const {
  return wb_motor_get_min_position(mContext, mTag);
}

double Motor::maxPosition() const {
  return wb_motor_get_max_position(mContext, mTag);
}

double Motor::position() const {
  WbDeviceTag sensor = wb_motor_get_position_sensor(mContext, mTag);
  if(sensor>=0)
  {
	  return wb_position_sensor_get_value(mContext, sensor);
  }
  else
  {
    if (wb_motor_get_type(mContext, mTag) == WB_ROTATIONAL)
      return wb_motor_get_torque_feedback(mContext, mTag);
    else
      return wb_motor_get_force_feedback(mContext, mTag);
  }
}

double Motor::targetPosition() const {
  return wb_motor_get_target_position(mContext, mTag);
}

void Motor::setPosition(double position) {
  wb_motor_set_position(mContext, mTag, position);
}
