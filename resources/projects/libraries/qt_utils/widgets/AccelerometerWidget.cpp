#include "AccelerometerWidget.hpp"
#include <devices/Device.hpp>

#include <webots/accelerometer.h>

using namespace webotsQtUtils;

AccelerometerWidget::AccelerometerWidget(Device *device, QWidget *parent) : VectorialSensorWidget(device, parent) {
}

void AccelerometerWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_accelerometer_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_accelerometer_disable(mDevice->context(), tag);
}

bool AccelerometerWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_accelerometer_get_sampling_period(mDevice->context(), tag) > 0;
}

const double *AccelerometerWidget::values() {
  WbDeviceTag tag = mDevice->tag();
  return wb_accelerometer_get_values(mDevice->context(), tag);
}
