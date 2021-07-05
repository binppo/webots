#include "CompassWidget.hpp"
#include <devices/Device.hpp>

#include <webots/compass.h>

using namespace webotsQtUtils;

CompassWidget::CompassWidget(Device *device, QWidget *parent) : VectorialSensorWidget(device, parent) {
}

void CompassWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_compass_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_compass_disable(mDevice->context(), tag);
}

bool CompassWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_compass_get_sampling_period(mDevice->context(), tag) > 0;
}

const double *CompassWidget::values() {
  WbDeviceTag tag = mDevice->tag();
  return wb_compass_get_values(mDevice->context(), tag);
}
