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

#ifndef WB_RADAR_HPP
#define WB_RADAR_HPP

#include "WbSolidDevice.hpp"
#include "utils/WbObjectDetection.hpp"

#include <QtCore/QDataStream>

#include <controller/c/messages.h>
#include <core/WbConfig.h>

class WbAffinePlane;
class WbSensor;
class WbRadarTargetObject;

struct WrTransform;
struct WrRenderable;
struct WrMaterial;
struct WrStaticMesh;

class WB_LIB_EXPORT WbRadar : public WbSolidDevice {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbRadar(WbTokenizer *tokenizer = NULL);
  WbRadar(const WbRadar &other);
  explicit WbRadar(const WbNode &other);
  virtual ~WbRadar() override;

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_RADAR; }
  void createWrenObjects() override;
  void preFinalize() override;
  void postFinalize() override;
  void handleMessage(QDataStream &) override;
  void writeAnswer(WbDataStream &) override;
  void writeConfigure(WbDataStream &) override;
  void prePhysicsStep(double ms) override;
  void postPhysicsStep() override;
  bool refreshSensorIfNeeded() override;
  void reset(const QString &id) override;

  // specific functions
  void rayCollisionCallback(dGeomID geom, WbSolid *collidingSolid, double depth);

  // field accessors
  double minRange() const { return mMinRange->value(); }
  double maxRange() const { return mMaxRange->value(); }
  double horizontalFieldOfView() const { return mHorizontalFieldOfView->value(); }
  double verticalFieldOfView() const { return mVerticalFieldOfView->value(); }
  double wavelength() const { return 0.299792458 / mFrequency->value(); }

  bool setTargetProperties(const WbVector3 &radarPosition, const WbMatrix3 &radarRotation, const WbVector3 &radarAxis,
                           const WbAffinePlane &radarPlane, WbRadarTargetObject *radarTarget);

  int refreshRate();

  QList<WbRadarTargetObject*> targets() const { return mRadarTargets; }

public slots:
  void SET_SAMPLING_PERIOD(int refreshRate);

private:
  // user accessible fields
  WbSFDouble *mMinRange;
  WbSFDouble *mMaxRange;
  WbSFDouble *mHorizontalFieldOfView;
  WbSFDouble *mVerticalFieldOfView;
  WbSFDouble *mMinAbsoluteRadialSpeed;
  WbSFDouble *mMinRadialSpeed;
  WbSFDouble *mMaxRadialSpeed;
  WbSFDouble *mCellDistance;
  WbSFDouble *mCellSpeed;
  WbSFDouble *mRangeNoise;
  WbSFDouble *mSpeedNoise;
  WbSFDouble *mAngularNoise;
  WbSFDouble *mAntennaGain;
  WbSFDouble *mFrequency;
  WbSFDouble *mTransmittedPower;
  WbSFBool *mOcclusion;
  WbSFDouble *mMinDetectableSignal;

  // precomputed values needed for target computation
  // converted transmittedPower from dBm to W
  double mTransmittedPowerInW;
  // converted antennaGain from dBi to to real gain
  double mRealGain;
  // converted minDetectableSignal from dBm into W
  double mReceivedPowerThreshold;
  double mReceivedPowerFactor;
  double mSensorElapsedTime;
  WbVector3 mPreviousRadarPosition;

  // other stuff
  WbSensor *mSensor;
  QList<WbRadarTargetObject *> mRadarTargets;
  QList<WbRadarTargetObject *> mInvalidRadarTargets;
  QMap<WbSolid *, WbVector3> mRadarTargetsPreviousTranslations;

  // WREN Data (for optional rendering)
  WrTransform *mTransform;
  WrRenderable *mRenderable;
  WrStaticMesh *mMesh;
  WrMaterial *mMaterial;

  // private functions
  WbRadar &operator=(const WbRadar &);  // non copyable
  WbNode *clone() const override { return new WbRadar(*this); }
  void init();
  void mergeTargets(int startingIndex = 0);
  void removeOccludedTargets();
  void updateRaysSetupIfNeeded() override;
  void updateReceivedPowerFactor();
  void computeTargets(bool finalSetup, bool needCollisionDetection);

private slots:
  void updateMinRange();
  void updateMaxRange();
  void updateHorizontalFieldOfView();
  void updateVerticalFieldOfView();
  void updateMinAbsoluteRadialSpeed();
  void updateMinAndMaxRadialSpeed();
  void updateCellDistance();
  void updateCellSpeed();
  void updateRangeNoise();
  void updateSpeedNoise();
  void updateAngularNoise();
  void updateFrequency();
  void updateAntennaGain();
  void updateTransmittedPower();
  void updateMinDetectableSignal();
  virtual void applyFrustumToWren();
  void updateOptionalRendering(int option);
};

class WB_LIB_EXPORT WbRadarTargetObject : public WbObjectDetection {
public:
  WbRadarTargetObject(WbRadar *radar, WbSolid *solidTarget, const bool needToCheckCollision, const double maxRange) :
    WbObjectDetection(radar, solidTarget, needToCheckCollision ? WbObjectDetection::ONE_RAY : WbObjectDetection::NONE, maxRange,
                      radar->horizontalFieldOfView()) {
    mTargetDistance = 0.0;
    mReceivedPower = 0.0;
    mSpeed = 0.0;
    mAzimuth = 0.0;
  };

  virtual ~WbRadarTargetObject() {}

  double targetDistance() const { return mTargetDistance; }
  double receivedPower() const { return mReceivedPower; }
  double speed() const { return mSpeed; }
  double azimuth() const { return mAzimuth; }
  void setTargetDistance(double distance) { mTargetDistance = distance; }
  void setReceivedPower(double receivedPower) { mReceivedPower = receivedPower; }
  void setSpeed(double speed) { mSpeed = speed; }
  void setAzimuth(double azimuth) { mAzimuth = azimuth; }

protected:
  double distance() override { return objectRelativePosition().length(); }

  double mTargetDistance;
  double mReceivedPower;
  double mSpeed;
  double mAzimuth;
};

#endif
