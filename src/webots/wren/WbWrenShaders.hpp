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

#ifndef WB_WREN_SHADERS_HPP
#define WB_WREN_SHADERS_HPP

#include <core/WbConfig.h>

struct WrShaderProgram;

namespace WbWrenShaders {
  WB_LIB_EXPORT WrShaderProgram *blendLensFlareShader();
  WB_LIB_EXPORT WrShaderProgram *bloomBlendShader();
  WB_LIB_EXPORT WrShaderProgram *brightPassShader();
  WB_LIB_EXPORT WrShaderProgram *boundingVolumeShader();
  WB_LIB_EXPORT WrShaderProgram *colorNoiseShader();
  WB_LIB_EXPORT WrShaderProgram *coordinateSystemShader();
  WB_LIB_EXPORT WrShaderProgram *defaultShader();
  WB_LIB_EXPORT WrShaderProgram *depthOfFieldShader();
  WB_LIB_EXPORT WrShaderProgram *depthOnlyShader();
  WB_LIB_EXPORT WrShaderProgram *depthResolutionShader();
  WB_LIB_EXPORT WrShaderProgram *encodeDepthShader();
  WB_LIB_EXPORT WrShaderProgram *fogShader();
  WB_LIB_EXPORT WrShaderProgram *gaussianBlurShader();
  WB_LIB_EXPORT WrShaderProgram *gaussianBlur5TapShader();
  WB_LIB_EXPORT WrShaderProgram *gaussianBlur9TapShader();
  WB_LIB_EXPORT WrShaderProgram *gaussianBlur13TapShader();
  WB_LIB_EXPORT WrShaderProgram *gtaoShader();
  WB_LIB_EXPORT WrShaderProgram *gtaoSpatialDenoiseShader();
  WB_LIB_EXPORT WrShaderProgram *gtaoTemporalDenoiseShader();
  WB_LIB_EXPORT WrShaderProgram *gtaoCombineShader();
  WB_LIB_EXPORT WrShaderProgram *handlesShader();
  WB_LIB_EXPORT WrShaderProgram *handlesPickingShader();
  WB_LIB_EXPORT WrShaderProgram *hdrClearShader();
  WB_LIB_EXPORT WrShaderProgram *hdrResolveShader();
  WB_LIB_EXPORT WrShaderProgram *iblDiffuseIrradianceBakingShader();
  WB_LIB_EXPORT WrShaderProgram *iblSpecularIrradianceBakingShader();
  WB_LIB_EXPORT WrShaderProgram *iblBrdfBakingShader();
  WB_LIB_EXPORT WrShaderProgram *iblEquirectangularShader();
  WB_LIB_EXPORT WrShaderProgram *lensDistortionShader();
  WB_LIB_EXPORT WrShaderProgram *lensFlareShader();
  WB_LIB_EXPORT WrShaderProgram *lightRepresentationShader();
  WB_LIB_EXPORT WrShaderProgram *lineSetShader();
  WB_LIB_EXPORT WrShaderProgram *mergeSphericalShader();
  WB_LIB_EXPORT WrShaderProgram *motionBlurShader();
  WB_LIB_EXPORT WrShaderProgram *noiseMaskShader();
  WB_LIB_EXPORT WrShaderProgram *overlayShader();
  WB_LIB_EXPORT WrShaderProgram *passThroughShader();
  WB_LIB_EXPORT WrShaderProgram *pbrShader();
  WB_LIB_EXPORT WrShaderProgram *pbrStencilAmbientEmissiveShader();
  WB_LIB_EXPORT WrShaderProgram *pbrStencilDiffuseSpecularShader();
  WB_LIB_EXPORT WrShaderProgram *phongShader();
  WB_LIB_EXPORT WrShaderProgram *phongStencilAmbientEmissiveShader();
  WB_LIB_EXPORT WrShaderProgram *phongStencilDiffuseSpecularShader();
  WB_LIB_EXPORT WrShaderProgram *pickingShader();
  WB_LIB_EXPORT WrShaderProgram *pointSetShader();
  WB_LIB_EXPORT WrShaderProgram *rangeNoiseShader();
  WB_LIB_EXPORT WrShaderProgram *shadowVolumeShader();
  WB_LIB_EXPORT WrShaderProgram *simpleShader();
  WB_LIB_EXPORT WrShaderProgram *skyboxShader();
  WB_LIB_EXPORT WrShaderProgram *smaaEdgeDetectionShader();
  WB_LIB_EXPORT WrShaderProgram *smaaBlendingWeightCalculationShader();
  WB_LIB_EXPORT WrShaderProgram *smaaFinalBlendShader();

  WB_LIB_EXPORT void deleteShaders();
};  // namespace WbWrenShaders

#endif
