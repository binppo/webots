/*
 * Description:  View all the supported webots devices in a tab based window sorted by categories
 */

#ifndef GENERIC_WINDOW_HPP
#define GENERIC_WINDOW_HPP

#include <gui/MainWindow.hpp>

#include <QtCore/QList>

struct WbRobotContext;
namespace webotsQtUtils {

  class CategoryWidget;
  class Device;
  class DeviceWidget;
  class MotionEditor;

  class GenericWindow : public MainWindow {
    Q_OBJECT

  public:
    explicit GenericWindow(WbRobotContext *ctx, QStringList hiddenDevices = QStringList());
    virtual ~GenericWindow();

    void readSensors();
    void writeActuators();

  protected:
    CategoryWidget *findCategory(const QString &name) const;

    MotionEditor *mMotionEditor;
    QTabWidget *mTabWidget;
    WbRobotContext *mContext;

    QList<Device *> mDeviceList;
    QList<DeviceWidget *> mDeviceWidgetList;
    QList<CategoryWidget *> mCategoryList;
  };
}  // namespace webotsQtUtils

#endif
