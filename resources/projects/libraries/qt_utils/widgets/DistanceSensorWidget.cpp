#include "DistanceSensorWidget.hpp"
#include <devices/Device.hpp>

#include <webots/distance_sensor.h>

using namespace webotsQtUtils;

DistanceSensorWidget::DistanceSensorWidget(Device *device, QWidget *parent) : ScalarSensorWidget(device, parent) {
}

void DistanceSensorWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_distance_sensor_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_distance_sensor_disable(mDevice->context(), tag);
}

bool DistanceSensorWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_distance_sensor_get_sampling_period(mDevice->context(), tag) > 0;
}

double DistanceSensorWidget::value() {
  WbDeviceTag tag = mDevice->tag();
  return wb_distance_sensor_get_value(mDevice->context(), tag);
}
