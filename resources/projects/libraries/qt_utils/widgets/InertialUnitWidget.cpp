#include "InertialUnitWidget.hpp"
#include <devices/Device.hpp>

#include <webots/inertial_unit.h>

using namespace webotsQtUtils;

InertialUnitWidget::InertialUnitWidget(Device *device, QWidget *parent) : VectorialSensorWidget(device, parent) {
}

void InertialUnitWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_inertial_unit_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_inertial_unit_disable(mDevice->context(), tag);
}

bool InertialUnitWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_inertial_unit_get_sampling_period(mDevice->context(), tag) > 0;
}

const double *InertialUnitWidget::values() {
  WbDeviceTag tag = mDevice->tag();
  return wb_inertial_unit_get_roll_pitch_yaw(mDevice->context(), tag);
}
