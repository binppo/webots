// Copyright 1996-2019 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
//  WbMotor.hpp
//

// Abstract class representing motors used to move mechanical joints

#ifndef WB_MOTOR_HPP
#define WB_MOTOR_HPP

#include "WbJointDevice.hpp"
#include "WbMFNode.hpp"
#include "WbSFDouble.hpp"
#include "WbSFVector3.hpp"

#include <core/WbConfig.h>

class WbSensor;
class WbSoundClip;

class WB_LIB_EXPORT WbMotor : public WbJointDevice {
  Q_OBJECT

public:
  virtual ~WbMotor();

  // Accessors
  bool userControl() const { return mUserControl; }
  double rawInput() const { return mRawInput; }
  double acceleration() const { return mAcceleration->value(); }
  const WbVector3 &controlPID() const { return mControlPID->value(); }
  double maxForceOrTorque() const { return mMaxForceOrTorque->value(); }
  double maxVelocity() const { return mMaxVelocity->value(); }
  double minPosition() const { return mMinPosition->value(); }
  double maxPosition() const { return mMaxPosition->value(); }
  void setMinPosition(double position) { mMinPosition->setValue(position); }
  void setMaxPosition(double position) { mMaxPosition->setValue(position); }
  const QString &sound() const { return mSound->value(); }
  const WbSoundClip *soundClip() const { return mSoundClip; }
  double computeCurrentDynamicVelocity(double ms, double position);
  bool runKinematicControl(double ms, double &position);
  double currentVelocity() const { return mCurrentVelocity; }
  double getTargetVelocity() const { return mTargetVelocity; }
  int kinematicVelocitySign() const { return mKinematicVelocitySign; }
  void setTargetPosition(double tp);
  void resetPhysics();
  double energyConsumption() const override;
  void powerOn(bool) override;

  bool isPIDPositionControl() const { return (!mUserControl && mMotorForceOrTorque != 0.0 && !std::isinf(mTargetPosition)); }

  bool hasMuscles() const { return !mMuscles->isEmpty(); }

  // inherited from WbDevice
  void preFinalize() override;
  void postFinalize() override;
  void createWrenObjects() override;
  void writeConfigure(QDataStream &stream) override;
  void handleMessage(QDataStream &stream) override;
  void writeAnswer(QDataStream &stream) override;
  bool refreshSensorIfNeeded() override;
  void reset() override;

  static const QList<const WbMotor *> &motors() { return cMotors; }

  double targetPosition() const { return mTargetPosition; }
  int getForceRefreshRate();
  double currentForce();
  double availableForce() const { return mMotorForceOrTorque; }

  bool fetchTransportQue(double &val);

public slots:
  void MOTOR_SET_POSITION(double position);
  void MOTOR_RESET_POSITION(double position);
  void MOTOR_SET_VELOCITY(double velocity);
  void MOTOR_SET_ACCELERATION(double acceleration);
  void MOTOR_SET_FORCE(double rawInput);
  void MOTOR_SET_AVAILABLE_FORCE(double force);
  void MOTOR_SET_CONTROL_PID(double controlP, double controlI, double controlD);
  void MOTOR_FEEDBACK(int rate);
  int MOTOR_GET_ASSOCIATED_DEVICE(int deviceType);

signals:
  void minPositionChanged();
  void maxPositionChanged();

protected:
  WbMotor(const QString &modelName, WbTokenizer *tokenizer = NULL);
  WbMotor(const WbMotor &other);
  WbMotor(const WbNode &other);

  // fields
  WbSFDouble *mMaxForceOrTorque;
  WbSFDouble *mMinPosition;
  WbSFDouble *mMaxPosition;

  virtual void turnOffMotor() = 0;
  double mMotorForceOrTorque;
  void enableMotorFeedback(int rate);
  virtual double computeFeedback() const = 0;

protected slots:
  void updateMaxForceOrTorque();
  void updateMinAndMaxPosition();

protected:
  void setupJointFeedback();

private:
  static QList<const WbMotor *> cMotors;

  void addConfigureToStream(QDataStream &stream);

  WbMotor &operator=(const WbMotor &);  // non copyable
  void init();
  WbSFDouble *mAcceleration;
  WbSFVector3 *mControlPID;
  WbSFDouble *mConsumptionFactor;
  WbSFDouble *mMaxVelocity;
  WbSensor *mForceOrTorqueSensor;
  double mForceOrTorqueLastValue;
  WbSFString *mSound;
  WbMFNode *mMuscles;
  WbSoundClip *mSoundClip;
  double mTargetVelocity;
  double mTargetPosition;
  double mCurrentVelocity;
  double mRawInput;
  bool mUserControl;
  bool mHasAllocatedJointFeedback;
  void setMaxAcceleration(double acc);
  void setMaxVelocity(double v);
  void awake() const;
  double mErrorIntegral;
  double mPreviousError;
  bool mNeedToConfigure;
  int mKinematicVelocitySign;
  QList<WbJointDevice *> mChangedAssociatedDevices;
  QList<double> mTransportQueue;
  WbDeviceTag *mRequestedDeviceTag;

private slots:
  void updateSound();
  void updateMaxVelocity();
  void updateMaxAcceleration();
  void updateControlPID();
  void updateMuscles();
};

#endif
