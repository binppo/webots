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

#ifndef WB_POINT_LIGHT_HPP
#define WB_POINT_LIGHT_HPP

#include "WbLight.hpp"

#include <QtCore/QMap>

#include <core/WbConfig.h>

class WbLightRepresentation;
class WbVector3;

struct WrPointLight;

class WB_LIB_EXPORT WbPointLight : public WbLight {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbPointLight(WbTokenizer *tokenizer = NULL);
  WbPointLight(const WbPointLight &other);
  explicit WbPointLight(const WbNode &other);
  virtual ~WbPointLight() override;

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_POINT_LIGHT; }
  void createWrenObjects() override;
  void preFinalize() override;
  void postFinalize() override;
  void reset(const QString &id) override;
  void save(const QString &id) override;

  // specific functions
  double computeAttenuation(double distance) const;
  WbVector3 computeAbsoluteLocation() const;

  QStringList fieldsToSynchronizeWithW3d() const override;

protected slots:
  void updateAmbientIntensity() override;
  // cppcheck-suppress uselessOverride
  void updateIntensity() override;
  void updateOn() override;

private:
  // user accessible fields
  WbSFVector3 *mAttenuation;
  WbSFVector3 *mLocation;
  WbSFDouble *mRadius;

  WrPointLight *mWrenLight;

  QMap<QString, WbVector3> mSavedLocation;

  // optional rendering
  WbLightRepresentation *mLightRepresentation;

  WbPointLight &operator=(const WbPointLight &);  // non copyable
  WbNode *clone() const override { return new WbPointLight(*this); }

  void init();
  void applyLightIntensityToWren() override;
  void applyLightColorToWren() override;
  void applyLightVisibilityToWren() override;
  void applyLightShadowsToWren() override;
  void applyLightAttenuationToWren();
  void applyNodeLocationToWren();
  void applyBillboardVisibilityToWren();
  void checkAmbientAndAttenuationExclusivity();

  void attachToUpperPose();
  void detachFromUpperPose();

private slots:
  void updateAttenuation();
  void updateLocation();
  void updateRadius();
  void updateOptionalRendering(int option);
};

#endif
