// Copyright 1996-2024 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WB_ROBOT_HPP
#define WB_ROBOT_HPP

#include "WbMFString.hpp"
#include "WbSFBool.hpp"
#include "WbSFString.hpp"
#include "WbSolid.hpp"
#include "WbVector3.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QVarLengthArray>
#include <QtCore/QVector>

#include <core/WbConfig.h>

class WbAbstractCamera;
class WbDataStream;
class WbDevice;
class WbJoystickInterface;
class WbKinematicDifferentialWheels;
class WbMFDouble;
class WbMouse;
class WbRenderingDevice;
class WbSensor;
class WbSupervisorUtilities;

class QByteArray;
class QDataStream;
class QTimer;

class WB_LIB_EXPORT WbRobot : public WbSolid {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbRobot(WbTokenizer *tokenizer = NULL);
  WbRobot(const WbRobot &other);
  explicit WbRobot(const WbNode &other);
  virtual ~WbRobot() override;

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_ROBOT; }
  void preFinalize() override;
  void postFinalize() override;
  void reset(const QString &id) override;
  void save(const QString &id) override;

  // controller
  void notifyExternControllerChanged();
  void newRemoteExternController();
  void removeRemoteExternController();
  bool isControllerExtern() const { return controllerName() == "<extern>"; }
  bool isControllerStarted() const { return mControllerStarted; }
  void startController();
  void setControllerStarted(bool started) {
    mControllerStarted = started;
    mControllerTerminated = false;
  }
  const QString &controllerDir();
  bool isConfigureDone() const { return !mConfigureRequest; }
  void restartController();
  void setControllerNeedRestart() { mNeedToRestartController = true; }
  bool isWaitingForUserInputEvent() const;
  bool isWaitingForWindow() const { return mWaitingForWindow; }
  void setWaitingForWindow(bool waiting);
  void addNewlyInsertedDevice(WbNode *node);
  void fixMissingResources() const override;

  // path to the project folder containing the proto model
  // returns an empty string if the robot is not a proto node
  QString protoModelProjectPath() const;

  // message dispatching
  virtual void powerOn(bool);
  bool isPowerOn() { return mPowerOn; }
  void dispatchMessage(QDataStream &);
  virtual void handleMessage(QDataStream &);
  virtual void writeAnswer(WbDataStream &);
  virtual bool hasImmediateAnswer() const;
  virtual void writeImmediateAnswer(WbDataStream &);
  void dispatchAnswer(WbDataStream &, bool includeDevices = true);
  void setConfigureRequest(bool b) { mConfigureRequest = b; }

  // device children
  int deviceCount() const { return mDevices.size(); }
  WbDevice *device(int index) const { return mDevices[index]; }
  WbDevice *findDevice(WbDeviceTag tag) const;
  void descendantNodeInserted(WbBaseNode *decendant) override;
  const QList<WbRenderingDevice *> &renderingDevices() { return mRenderingDevices; }

  // update sensors in case of no answer needs to be written at this step
  virtual void updateSensors();

  void renderCameras();

  // field accessors
  const QString &controllerName() const { return mController->value(); }
  const QStringList &controllerArgs() const { return mControllerArgs->value(); }
  const QString &customData() const { return mCustomData->value(); }
  const QString &window() const { return mWindow->value(); }
  bool synchronization() const { return mSynchronization->value(); }
  bool supervisor() const { return mSupervisor->value(); }
  const WbMFDouble &battery() const { return *mBattery; }
  bool selfCollision() const { return mSelfCollision->value(); }

  void setSupervisor(bool enable);

  WbSupervisorUtilities *supervisorUtilities() const { return mSupervisorUtilities; }

  const bool isRobot() const override { return true; };

  // energy accessors and setters
  double currentEnergy() const;
  void setCurrentEnergy(double e);
  double maxEnergy() const;
  double energyUploadSpeed() const;

  // handle key events
  void keyPressed(int key, int modifiers);
  void keyReleased(int key);

  // map qt special key to webots special key, return 0 if not found
  static int mapSpecialKey(int qtKey);
  // return the absolute file name of the robot window file, if it exists
  QString windowFile(const QString &extension = "html") const;
  void showWindow();  // show the Qt-based controller robot window (to be deprecated)
  void updateControllerWindow();

  void processImmediateMessages();

  void setNeedToWriteUserInputEventAnswer() { mNeedToWriteUserInputEventAnswer = true; }

  WbKinematicDifferentialWheels *kinematicDifferentialWheels() { return mKinematicDifferentialWheels; }

  QString encodedName() const;  // name used for controller connections

public slots:
  void SET_URDF(const QString& data);
  QString GET_URDF() const;
  void SET_BATTERY_SAMPLING_PERIOD(int refreshRate);
  void SET_DATA(const QString& data);
  void SET_KEYBOARD_SAMPLING_PERIOD(int refreshRate);
  void SET_JOYSTICK_SAMPLING_PERIOD(int refreshRate);
  void SET_MOUSE_SAMPLING_PERIOD(int refreshRate);
  void MOUSE_ENABLE_3D_POSITION(bool enable);
  void SET_JOYSTICK_FORCE_FEEDBACK(int level);
  void SET_JOYSTICK_FORCE_FEEDBACK_DURATION(double duration);
  void SET_JOYSTICK_AUTO_CENTERING_GAIN(double gain);
  void SET_JOYSTICK_RESISTANCE_GAIN(double gain);
  void SET_JOYSTICK_FORCE_AXIS(int axis);
  void REMOTE_ON();
  void REMOTE_OFF();
  void PIN(bool pin);
  void CONSOLE_MESSAGE(QString message, bool streamChannel);
  void WWI_MESSAGE(const QByteArray& message);

  void receiveFromJavascript(const QByteArray &message);
  void updateControllerDir();

  void pinToStaticEnvironment(bool pin);

signals:
  void startControllerRequest(WbRobot *robot);
  void immediateMessageAdded();
  void externControllerChanged();
  void controllerChanged();
  void controllerExited();
  void windowChanged();
  void wasReset();
  void toggleRemoteMode(bool enable);
  void sendToJavascript(const QByteArray &);
  void appendMessageToConsole(const QString &message, bool useStdout);
  void userInputEventNeedUpdate();
  void keyboardChanged();
  void windowReady();

protected:
  WbRobot(const QString &modelName, WbTokenizer *tokenizer);

  // reimplemented protected functions
  void prePhysicsStep(double ms) override;
  void postPhysicsStep() override;
  virtual void writeConfigure(WbDataStream &);

  // export
  const QString urdfName() const override;

  WbKinematicDifferentialWheels *mKinematicDifferentialWheels;

private:
  // user accessible fields
  WbSFString *mController;
  WbMFString *mControllerArgs;
  WbSFString *mCustomData;
  WbSFBool *mSupervisor;
  WbSFBool *mSynchronization;
  WbMFDouble *mBattery;
  WbSFDouble *mCpuConsumption;
  WbSFBool *mSelfCollision;
  WbSFString *mWindow;
  WbSFString *mRemoteControl;

  bool mNeedToWriteUrdf;
  bool mShowWindowCalled;
  bool mShowWindowMessage;
  bool mUpdateWindowMessage;
  bool mWaitingForWindow;
  QByteArray *mMessageFromWwi;
  bool mDataNeedToWriteAnswer;
  bool mSupervisorNeedToWriteAnswer;
  bool mModelNeedToWriteAnswer;
  bool mPowerOn;
  bool mControllerStarted;
  bool mControllerTerminated;
  bool mNeedToRestartController;
  bool mConfigureRequest;
  bool mSimulationModeRequested;

  QString mControllerDir;

  double mPreviousTime;

  // supervisor
  bool mSupervisorUtilitiesNeedUpdate;
  WbSupervisorUtilities *mSupervisorUtilities;

  // pin
  bool mPin;
  WbVector3 mPinTranslation;
  WbRotation mPinRotation;

  // dynamic libraries
  QString mAbsoluteWindowFilename;
  QString mAbsoluteRemoteControlFilename;

  // sensors
  WbSensor *mBatterySensor;
  WbSensor *mKeyboardSensor;
  WbSensor *mJoystickSensor;
  double mBatteryLastValue;
  QMap<QString, double> mBatteryInitialValues;
  QList<int> mKeyboardLastValue;
  struct JoyStickLastValue {
    int numberOfPressedButtons;
    QList<int> pressedButtonsIndices;
    int numberOfAxes;
    QList<int> axesValues;
    int numberOfPovs;
    QList<int> povsValues;
  };
  JoyStickLastValue *mJoyStickLastValue;
  WbMouse *mMouse;

  // if sensor refresh is needed, update value and return TRUE
  bool refreshBatterySensorIfNeeded();
  bool refreshKeyboardSensorIfNeeded();
  bool refreshJoyStickSensorIfNeeded();

  // joystick interface
  WbJoystickInterface *mJoystickInterface;
  bool mJoystickConfigureRequest;
  QTimer *mJoystickTimer;

  // user input events
  QTimer *mUserInputEventTimer;
  int mMonitoredUserInputEventTypes;
  QDateTime mUserInputEventReferenceTime;
  bool mNeedToWriteUserInputEventAnswer;
  bool mKeyboardHasChanged;

  // other variables
  QList<WbDevice *> mDevices;
  QList<WbRenderingDevice *> mRenderingDevices;
  QList<WbAbstractCamera *> mActiveCameras;
  QList<WbDevice *> mNewlyAddedDevices;
  int mNextTag;

  QList<int> mPressedKeys;

  WbRobot &operator=(const WbRobot &);  // non copyable
  WbNode *clone() const override { return new WbRobot(*this); }
  void init();
  void addDevices(WbNode *node);
  // if reset is TRUE reassign tags to devices (when device config changed)
  // if reset is FALSE, only tag of newly added devices will be assigned
  void assignDeviceTags(bool reset);
  void writeDeviceConfigure(QList<WbDevice *> devices, WbDataStream &stream) const;
  QString searchDynamicLibraryAbsolutePath(const QString &key, const QString &pluginSubdirectory);
  void updateDevicesAfterInsertion();
  void updateControllerStatusInDevices();
  double energyConsumption() const;
  void clearDevices();
  int computeSimulationMode();

private slots:
  void updateDevicesAfterDestruction();
  void updateActiveCameras(WbAbstractCamera *camera, bool isActive);
  void updateWindow();
  void updateRemoteControl();
  void updateSimulationMode();
  void updateData();
  void updateSupervisor();
  void updateModel();
  void updateBattery(bool itemInserted);
  void removeRenderingDevice();
  void handleMouseChange();
  void handleJoystickChange();
};

#endif
