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

#ifndef WB_FILE_UTIL_HPP
#define WB_FILE_UTIL_HPP

//
// Description: useful file operations for Webots
//

#include <QtCore/QString>

#include <core/WbConfig.h>

namespace WbFileUtil {

  // copy file from 'sourcePath' to 'destPath' while replacing a string pattern
  WB_LIB_EXPORT bool copyAndReplaceString(const QString &sourcePath, const QString &destinationPath,
                            const QList<std::pair<QString, QString>> &values);
  WB_LIB_EXPORT bool copyAndReplaceString(const QString &sourcePath, const QString &destinationPath, const QString &before,
                            const QString &after);

  // force copy a file (remove the target if existing)
  WB_LIB_EXPORT bool forceCopy(const QString &source, const QString &target);

  // copy all files from 'sourcePath' to 'destPath'
  // return the number of files copied or 0 if there was a problem
  WB_LIB_EXPORT int copyDir(const QString &sourcePath, const QString &destPath, bool recurse, bool merge, bool setWritable = false);

  // check if 2 files are identical
  WB_LIB_EXPORT bool areIdenticalFiles(const QString &fileAPath, const QString &fileBPath);

  // check if a file is located in the specified directory tree; both paths are absolute
  WB_LIB_EXPORT bool isLocatedInDirectory(const QString &file, const QString &directory);
  WB_LIB_EXPORT bool isLocatedInInstallationDirectory(const QString &file, bool ignoreAllowModify = false);

  // search all the directories named directoryName in the root directory
  WB_LIB_EXPORT void searchDirectoryNameRecursively(QStringList &results, const QString &directoryName, const QString &root);

  // check if the directory is writable
  // if it doesn't exists, check if it can be created (i.e. if parent directory is writable)
  WB_LIB_EXPORT bool isDirectoryWritable(const QString &path);

  enum FileType { EXECUTABLE, CLASS, JAR, PYTHON, MATLAB, BOTSTUDIO, DOCKER, UNKNOWN };

  WB_LIB_EXPORT void revealInFileManager(const QString &file);
};  // namespace WbFileUtil

#endif
