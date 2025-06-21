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

#ifndef WB_INERTIAL_UNIT_HPP
#define WB_INERTIAL_UNIT_HPP

#include "WbSolidDevice.hpp"

#include <QtCore/QDataStream>
#include <QtGui/QQuaternion>

#include <controller/c/messages.h>
#include <core/WbConfig.h>

class WbSensor;
class WbMFVector3;
class WbSFBool;

class WB_LIB_EXPORT WbInertialUnit : public WbSolidDevice {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbInertialUnit(WbTokenizer *tokenizer = NULL);
  WbInertialUnit(const WbInertialUnit &other);
  explicit WbInertialUnit(const WbNode &other);
  virtual ~WbInertialUnit() override;

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_INERTIAL_UNIT; }
  void preFinalize() override;
  void postFinalize() override;
  void handleMessage(QDataStream &) override;
  void writeAnswer(WbDataStream &) override;
  void writeConfigure(WbDataStream &) override;
  bool refreshSensorIfNeeded() override;

  int refreshRate();
  double noise();

public slots:
  void SET_SAMPLING_PERIOD(int refreshRate);

  QQuaternion value();

private:
  // user accessible fields
  WbSFBool *mXAxis, *mYAxis, *mZAxis;
  WbSFDouble *mResolution;
  WbSFDouble *mNoise;

  // other stuff
  WbSensor *mSensor;
  WbQuaternion mQuaternion;
  bool mNeedToReconfigure;

  // private functions
  WbInertialUnit &operator=(const WbInertialUnit &);  // non copyable
  WbNode *clone() const override { return new WbInertialUnit(*this); }
  void init();
  void computeValue();
  void addConfigure(WbDataStream &);

private slots:
  void updateResolution();
  void updateNoise();
};

#endif
