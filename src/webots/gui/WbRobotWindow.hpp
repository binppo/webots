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

#ifndef WB_ROBOT_WINDOW_HPP
#define WB_ROBOT_WINDOW_HPP

#include "WbDockWidget.hpp"
#include "WbRobot.hpp"

#include <core/WbConfig.h>

class WbRobotWindowTransportLayer;

class QWebFrame;
class QWebEngineView;

class WB_LIB_EXPORT WbRobotWindow : public WbDockWidget {
  Q_OBJECT

public:
  explicit WbRobotWindow(WbRobot *, QWidget *parent = NULL);
  virtual ~WbRobotWindow();
  WbRobot *robot() { return mRobot; }
  const QString *name() { return &mRobot->window(); }
  void show();

public slots:
  void sendToJavascript(const QByteArray &);
  void setTitle(const QString &title, const QString &tabbedTitle = NULL);
  void startControllerIfNeeded();

private:
  QString formatUrl(const QString &urlString);
  QString linkTag(const QString &file);
  QString scriptTag(const QString &file);
  WbRobot *mRobot;
  QWebEngineView *mWebView;
  void runJavaScript(const QString &message);
  QStringList mWaitingSentMessages;
  WbRobotWindowTransportLayer *mTransportLayer;
  int mResetCount;
  bool mLoaded;

private slots:
  void notifyLoadCompleted();
  void notifyAckReceived();
  void setupPage();
};

#endif  // WB_ROBOT_WINDOW_HPP
