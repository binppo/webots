#include "GPSWidget.hpp"
#include <devices/Device.hpp>

#include <webots/gps.h>

using namespace webotsQtUtils;

GPSWidget::GPSWidget(Device *device, QWidget *parent) : VectorialSensorWidget(device, parent) {
}

void GPSWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_gps_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_gps_disable(mDevice->context(), tag);
}

bool GPSWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_gps_get_sampling_period(mDevice->context(), tag) > 0;
}

const double *GPSWidget::values() {
  WbDeviceTag tag = mDevice->tag();
  return wb_gps_get_values(mDevice->context(), tag);
}
