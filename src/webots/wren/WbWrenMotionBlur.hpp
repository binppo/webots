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

#ifndef WB_WREN_MOTION_BLUR_HPP
#define WB_WREN_MOTION_BLUR_HPP

#include "WbWrenAbstractPostProcessingEffect.hpp"

#include <core/WbConfig.h>

class WB_LIB_EXPORT WbWrenMotionBlur : public WbWrenAbstractPostProcessingEffect {
public:
  WbWrenMotionBlur();

  void setup(WrViewport *viewport) override;
  void setFirstRender(bool firstRender);
  void setIntensity(float intensity);

private:
  void applyParametersToWren() override;

  bool mFirstRender;
  float mIntensity;
};

#endif  // WB_WREN_MOTION_BLUR_HPP
