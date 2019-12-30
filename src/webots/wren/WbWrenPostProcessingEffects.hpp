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

#ifndef WB_WREN_POST_PROCESSING_EFFECTS_HPP
#define WB_WREN_POST_PROCESSING_EFFECTS_HPP

struct WrPostProcessingEffect;

#include <wren/texture.h>

#include <core/WbConfig.h>

namespace WbWrenPostProcessingEffects {
  WB_LIB_EXPORT void loadResources();
  WB_LIB_EXPORT void clearResources();
  WB_LIB_EXPORT WrPostProcessingEffect *lensFlare(float width, float height, int blurIterations);
  WB_LIB_EXPORT WrPostProcessingEffect *sphericalCameraMerge(float width, float height, int cameraCount,
                                               WrTextureInternalFormat textureFormat);

  WB_LIB_EXPORT WrPostProcessingEffect *lensDistortion(float width, float height, WrTextureInternalFormat textureFormat);
  WB_LIB_EXPORT WrPostProcessingEffect *depthOfField(float width, float height, float depthOfFieldTextureWidth,
                                       float depthOfFieldTextureHeight, WrTextureInternalFormat textureFormat,
                                       WrTexture *colorTexture, WrTexture *depthTexture);
  WB_LIB_EXPORT WrPostProcessingEffect *motionBlur(float width, float height, WrTextureInternalFormat textureFormat);
  WB_LIB_EXPORT WrPostProcessingEffect *colorNoise(float width, float height, WrTextureInternalFormat textureFormat);
  WB_LIB_EXPORT WrPostProcessingEffect *rangeNoise(float width, float height, WrTextureInternalFormat textureFormat);
  WB_LIB_EXPORT WrPostProcessingEffect *depthResolution(float width, float height, WrTextureInternalFormat textureFormat);
  WB_LIB_EXPORT WrPostProcessingEffect *noiseMask(float width, float height, WrTextureInternalFormat textureFormat,
                                    WrTexture *noiseMaskTexture);
  WB_LIB_EXPORT WrPostProcessingEffect *smaa(float width, float height, WrTextureInternalFormat textureFormat);
  WB_LIB_EXPORT WrPostProcessingEffect *gtao(float width, float height, WrTextureInternalFormat textureFormat, WrTexture *depthTexture,
                               WrTexture *normalTexture, bool halfRes);
  WB_LIB_EXPORT WrPostProcessingEffect *bloom(float width, float height, WrTextureInternalFormat textureFormat);
  WB_LIB_EXPORT WrPostProcessingEffect *hdrResolve(float width, float height);

};  // namespace WbWrenPostProcessingEffects

#endif
