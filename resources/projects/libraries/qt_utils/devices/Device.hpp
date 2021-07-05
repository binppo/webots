/*
 * Description:  Abstraction of a Webots robot device
 */

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <webots/device.h>
#include <webots/robot.h>

#include <QtCore/QString>

struct WbRobotContext;
namespace webotsQtUtils {
  class Device {
  public:
    explicit Device(WbRobotContext *ctx, WbDeviceTag tag);
    virtual ~Device() {}

    WbDeviceTag tag() const { return mTag; }
    WbNodeType type() const { return mType; }
    const QString &name() const { return mName; }
    const QString &category() const { return mCategory; }

    WbRobotContext *context() const { return mContext; }

  protected:
    WbDeviceTag mTag;
    WbNodeType mType;
    WbRobotContext *mContext;
    QString mName;
    QString mCategory;
  };
}  // namespace webotsQtUtils

#endif
