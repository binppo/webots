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

#ifndef WB_TOUCH_SENSOR_HPP
#define WB_TOUCH_SENSOR_HPP

#include "WbSolidDevice.hpp"

#include <QtCore/QDataStream>
#include <QtGui/QVector3D>

#include <controller/c/messages.h>
#include <core/WbConfig.h>

class WbSensor;
class WbLookupTable;
struct dJointFeedback;

class WB_LIB_EXPORT WbTouchSensor : public WbSolidDevice {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbTouchSensor(WbTokenizer *tokenizer = NULL);
  WbTouchSensor(const WbTouchSensor &other);
  explicit WbTouchSensor(const WbNode &other);
  virtual ~WbTouchSensor() override;

  void setTouching(bool touching) { mIsTouching = touching; }
  void setGuiTouch(bool touching) { mIsGuiTouch = touching; }

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_TOUCH_SENSOR; }
  enum Type { BUMPER, FORCE, FORCE3D };
  virtual Type deviceType() { return mDeviceType; }
  void preFinalize() override;
  void postFinalize() override;
  void handleMessage(QDataStream &) override;
  void writeAnswer(WbDataStream &) override;
  void writeConfigure(WbDataStream &) override;
  void setSolidMerger() override;
  void createOdeObjects() override;
  bool refreshSensorIfNeeded() override;

  int lookupTableSize() const;
  QVector<QVector3D> lookupTable() const;

  int refreshRate();

public slots:
  void SET_SAMPLING_PERIOD(int refreshRate);

  QList<double> value();

protected:
  // reimplemented protected functions
  dJointID createJoint(dBodyID body, dBodyID parentBody, dWorldID world) const override;
  bool needJointToUpperSolid(const WbSolid *upperSolid) const override { return true; }

private:
  Type mDeviceType;

  // user accessible fields
  WbMFVector3 *mLookupTable;
  WbSFString *mType;
  WbSFDouble *mResolution;

  // other stuff
  bool mIsTouching;
  bool mIsGuiTouch;
  WbSensor *mSensor;
  WbLookupTable *mLut;
  double mValues[3];                  // current sensor value according to lookup table
  mutable dJointFeedback *mFeedback;  // for measuring force feedback on the fixed joint
  bool mNeedToReconfigure;

  // private functions
  WbTouchSensor &operator=(const WbTouchSensor &);  // non copyable
  WbNode *clone() const override { return new WbTouchSensor(*this); }
  void init();
  void computeValue();
  bool forceBehavior() const;
  void setODEDynamicFlag(const WbBaseNode *_node);
  void addConfigure(WbDataStream &);

private slots:
  void updateLookupTable();
  void updateType();
  void updateResolution();
};

#endif
