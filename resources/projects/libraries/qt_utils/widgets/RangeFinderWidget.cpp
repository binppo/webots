#include "RangeFinderWidget.hpp"
#include <devices/Device.hpp>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include <webots/range_finder.h>

using namespace webotsQtUtils;

RangeFinderWidget::RangeFinderWidget(Device *device, QWidget *parent) : SensorWidget(device, parent) {
  mHBox = new QHBoxLayout(mMainWidget);
  mLabel = new QLabel(this);
  mLabel->setAlignment(Qt::AlignCenter);

  mHBox->addWidget(mLabel);
  mMainWidget->setLayout(mHBox);
}

void RangeFinderWidget::readSensors() {
  SensorWidget::readSensors();

  WbDeviceTag tag = mDevice->tag();
  if (wb_range_finder_get_sampling_period(mDevice->context(), tag) > 0) {
    int rangeFinderWidth = wb_range_finder_get_width(mDevice->context(), tag);
    int rangeFinderHeight = wb_range_finder_get_height(mDevice->context(), tag);
    const float *raw = wb_range_finder_get_range_image(mDevice->context(), tag);
    if (!raw || rangeFinderWidth < 1 || rangeFinderHeight < 1)
      return;

    int labelWidth = mLabel->width();
    int labelHeight = mLabel->height();
    double rangeFinderRatio = (double)rangeFinderWidth / (double)rangeFinderHeight;
    double labelRatio = (double)labelWidth / (double)labelHeight;
    QImage *image;
    unsigned char *buffer = new unsigned char[rangeFinderWidth * rangeFinderHeight * 4];
    int k = 0, r = 0;
    int size = rangeFinderWidth * rangeFinderHeight;
    float _255OverMax = 255.0f / wb_range_finder_get_max_range(mDevice->context(), tag);
    while (r < size) {
      buffer[k++] = raw[r] * _255OverMax;
      buffer[k++] = raw[r] * _255OverMax;
      buffer[k++] = raw[r++] * _255OverMax;
      buffer[k++] = 0xFF;
    }
    image = new QImage(buffer, rangeFinderWidth, rangeFinderHeight, 4 * rangeFinderWidth, QImage::Format_ARGB32);
    QPixmap pixmap(QPixmap::fromImage(*image));

    if (rangeFinderRatio > labelRatio)
      mLabel->setPixmap(pixmap.scaledToWidth(labelWidth));
    else
      mLabel->setPixmap(pixmap.scaledToHeight(labelHeight));
    delete image;
    delete[] buffer;
  }
}

void RangeFinderWidget::enable(bool enable) {
  WbDeviceTag tag = mDevice->tag();
  if (enable)
    wb_range_finder_enable(mDevice->context(), tag, static_cast<int>(wb_robot_get_basic_time_step(mDevice->context())));
  else
    wb_range_finder_disable(mDevice->context(), tag);
}

bool RangeFinderWidget::isEnabled() const {
  WbDeviceTag tag = mDevice->tag();
  return wb_range_finder_get_sampling_period(mDevice->context(), tag) > 0;
}
