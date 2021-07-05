#include "GyroWidget.hpp"
#include <devices/Device.hpp>

#include <webots/gyro.h>

using namespace webotsQtUtils;

GyroWidget::GyroWidget(Device *device, QWidget *parent) : VectorialSensorWidget(device, parent) {
}

void GyroWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_gyro_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_gyro_disable(mDevice->context(), tag);
}

bool GyroWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_gyro_get_sampling_period(mDevice->context(), tag) > 0;
}

const double *GyroWidget::values() {
  WbDeviceTag tag = mDevice->tag();
  return wb_gyro_get_values(mDevice->context(), tag);
}
