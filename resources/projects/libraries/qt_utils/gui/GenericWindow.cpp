#include "GenericWindow.hpp"

#include <motion_editor/MotionEditor.hpp>
#include <motion_editor/MotionGlobalSettings.hpp>

#include <devices/Motor.hpp>

#include <widgets/CategoryWidget.hpp>
#include <widgets/DeviceWidget.hpp>
#include <widgets/DeviceWidgetFactory.hpp>

using namespace webotsQtUtils;

GenericWindow::GenericWindow(WbRobotContext *ctx, QStringList hiddenDevices) : MainWindow(), mContext(ctx), mMotionEditor(NULL) {
  setStyleSheet("QWidget#borderedWidget { "
                "  border: 2px groove darkGray; "
                "  border-radius: 8px; "
                "} "
                "QListWidget::item:selected:active:!enabled { "
                "  background: darkGray; "
                "} "
                "QPushButton#menuButton { "
                "  width: 25px; "
                "  height: 25px; "
                "  icon-size: 22px; "
                "  padding: 3px; "
                "  border: 1px solid transparent; "
                "  border-radius: 5px; "
                "} "
                "QPushButton#menuButton:hover { "
                "  border: 1px solid gray; "
                "  background-color: rgba(180, 180, 255, 50%); "
                "} "
                "QPushButton#menuButton:pressed { "
                "  border: 1px solid gray; "
                "  background-color: rgba(0, 0, 0, 25%); "
                "} "
#ifdef __linux__
                // original group box bounds are not clear
                "QGroupBox#borderedGroupBox { "
                "  border: 2px groove darkGray; "
                "  border-radius: 8px; "
                "  margin-top: 1ex; "
                "  padding: 5px; "
                "} "
                "QGroupBox#borderedGroupBox::title { "
                "  subcontrol-origin: margin; "
                "  subcontrol-position: top center; "
                "  padding: 0 3px; "
                "} "
#endif
  );
  setFixedWidth(810);

  QString title(tr("Robot Window: %1").arg(wb_robot_get_name(mContext)));
  setWindowTitle(title);

  mTabWidget = new QTabWidget(this);
  mTabWidget->setMovable(true);

  QList<webotsQtUtils::Motor *> motorList;

  mDeviceList << new Device(mContext, (WbDeviceTag)0);  // robot device
  for (int index = 0; index < wb_robot_get_number_of_devices(mContext); index++) {
    WbDeviceTag tag = wb_robot_get_device_by_index(mContext, index);
    if (hiddenDevices.contains(wb_device_get_name(mContext, tag)))
      continue;
    WbNodeType type = wb_device_get_node_type(mContext, tag);
    if (type == WB_NODE_LINEAR_MOTOR || type == WB_NODE_ROTATIONAL_MOTOR) {
      Motor *motor = new Motor(mContext, tag);
      mDeviceList << motor;
      motorList << motor;
    } else {
      Device *device = new Device(mContext, tag);
      mDeviceList << device;
    }
  }

  foreach (Device *device, mDeviceList) {
    const QString &categoryName = device->category();
    if (categoryName != "Unknown") {
      CategoryWidget *category = findCategory(categoryName);
      if (category == NULL) {
        category = new CategoryWidget(categoryName, this);
        mCategoryList << category;
        mTabWidget->addTab(category, categoryName);
      }
      DeviceWidget *widget = DeviceWidgetFactory::createDeviceWidget(device, this);
      if (widget) {
        mDeviceWidgetList << widget;
        category->addWidget(widget);
      }
    }
  }

  // motion editor
  if (motorList.size() > 0) {
    MotionGlobalSettings::setAvailableMotorList(motorList);
    mMotionEditor = new MotionEditor(mContext, this);
    mTabWidget->addTab(mMotionEditor, tr("Motion Editor"));
  }

  setCentralWidget(mTabWidget);
}

GenericWindow::~GenericWindow() {
  mDeviceWidgetList.clear();
  mCategoryList.clear();

  foreach (Device *device, mDeviceList)
    delete device;
  mDeviceList.clear();
}

void GenericWindow::readSensors() {
  foreach (DeviceWidget *widget, mDeviceWidgetList)
    widget->readSensors();
}

void GenericWindow::writeActuators() {
  foreach (DeviceWidget *widget, mDeviceWidgetList)
    widget->writeActuators();

  if (mMotionEditor)
    mMotionEditor->writeActuators();
}

CategoryWidget *GenericWindow::findCategory(const QString &name) const {
  foreach (CategoryWidget *category, mCategoryList) {
    if (category->name() == name)
      return category;
  }
  return NULL;
}
