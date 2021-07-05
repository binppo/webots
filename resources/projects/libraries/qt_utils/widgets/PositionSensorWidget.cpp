#include "PositionSensorWidget.hpp"
#include <devices/Device.hpp>

#include <webots/position_sensor.h>

using namespace webotsQtUtils;

PositionSensorWidget::PositionSensorWidget(Device *device, QWidget *parent) : ScalarSensorWidget(device, parent) {
}

void PositionSensorWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_position_sensor_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_position_sensor_disable(mDevice->context(), tag);
}

bool PositionSensorWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_position_sensor_get_sampling_period(mDevice->context(), tag) > 0;
}

double PositionSensorWidget::value() {
  WbDeviceTag tag = mDevice->tag();
  return wb_position_sensor_get_value(mDevice->context(), tag);
}
