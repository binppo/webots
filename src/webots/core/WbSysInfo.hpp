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

#ifndef WB_SYS_INFO_HPP
#define WB_SYS_INFO_HPP

#include <QtCore/QString>

#include <core/WbConfig.h>

class QOpenGLFunctions;

namespace WbSysInfo {
  enum WbPlatform { LINUX_PLATFORM, MACOS_PLATFORM, WIN32_PLATFORM };

  WB_LIB_EXPORT const QString &sysInfo();
  WB_LIB_EXPORT const QString &platformShortName();  // either "windows", "mac", "linux32" or "linux64"
  WB_LIB_EXPORT const QString &processor();
  WB_LIB_EXPORT bool isVirtualMachine();
#ifdef __linux__
  WB_LIB_EXPORT const QString &linuxCpuModelName();
  WB_LIB_EXPORT bool isRootUser();
  WB_LIB_EXPORT inline bool isSnap() {
    return qgetenv("SNAP_NAME") == "webots";
  }
#else
  WB_LIB_EXPORT inline bool isSnap() {
    return false;
  }
#endif
  WB_LIB_EXPORT QString environmentVariable(const QString &name);
  WB_LIB_EXPORT void setEnvironmentVariable(const QString &name, const QString &value);
  WB_LIB_EXPORT QString shortPath(const QString &path);  // Windows 8.3 short path name

  WB_LIB_EXPORT WbPlatform platform();

  WB_LIB_EXPORT bool isPointerSize32bits();
  WB_LIB_EXPORT bool isPointerSize64bits();

  WB_LIB_EXPORT int coreCount();

#ifdef _WIN32
  WB_LIB_EXPORT quint32 gpuDeviceId(QOpenGLFunctions *gl);
  WB_LIB_EXPORT quint32 gpuVendorId(QOpenGLFunctions *gl);
  WB_LIB_EXPORT int intelGPUGeneration(QOpenGLFunctions *gl);
  WB_LIB_EXPORT bool isAmdLowEndGpu(QOpenGLFunctions *gl);
#else
  WB_LIB_EXPORT bool isLowEndGpu();
#endif
  WB_LIB_EXPORT const void initializeOpenGlInfo();
  WB_LIB_EXPORT const QString &openGLRenderer();
  WB_LIB_EXPORT const QString &openGLVendor();
  WB_LIB_EXPORT const QString &openGLVersion();
  WB_LIB_EXPORT void openGlLineWidthRange(double &min, double &max);
};  // namespace WbSysInfo

#endif
