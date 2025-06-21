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

#ifndef WB_URL_HPP
#define WB_URL_HPP

#include <QtCore/QString>

#include <core/WbConfig.h>

class WbNode;
class WbMFString;
class WbWriter;

namespace WbUrl {
  WB_LIB_EXPORT QString resolveUrl(const QString &rawUrl);
  WB_LIB_EXPORT QString computePath(const WbNode *node, const QString &field, const QString &rawUrl, bool showWarning = false);
  WB_LIB_EXPORT QString computePath(const WbNode *node, const QString &field, const WbMFString *urlField, int index,
                      bool showWarning = false);

  WB_LIB_EXPORT QString combinePaths(const QString &rawUrl, const QString &rawParentUrl);

  WB_LIB_EXPORT QString exportResource(const WbNode *node, const QString &url, const QString &sourcePath, const QString &relativeResourcePath,
                         const WbWriter &writer, const bool isTexture = true);
  WB_LIB_EXPORT QString exportTexture(const WbNode *node, const WbMFString *urlField, int index, const WbWriter &writer);
  WB_LIB_EXPORT QString exportMesh(const WbNode *node, const WbMFString *urlField, int index, const WbWriter &writer);

  WB_LIB_EXPORT QString missing(const QString &url);
  WB_LIB_EXPORT const QString &missingTexture();
  WB_LIB_EXPORT const QString &missingProtoIcon();
  WB_LIB_EXPORT bool isWeb(const QString &url);
  WB_LIB_EXPORT bool isLocalUrl(const QString &url);
  WB_LIB_EXPORT QString computeLocalAssetUrl(QString url, bool isW3d);
  WB_LIB_EXPORT QString computePrefix(const QString &rawUrl);

  WB_LIB_EXPORT QString remoteWebotsAssetRegex(bool capturing);
  WB_LIB_EXPORT const QString &remoteWebotsAssetPrefix();

  WB_LIB_EXPORT const QRegularExpression &vrmlResourceRegex();

  WB_LIB_EXPORT QString expressRelativeToWorld(const QString &url);

  WB_LIB_EXPORT void setWorldFileName(const QString &fileName);

};  // namespace WbUrl

#endif
