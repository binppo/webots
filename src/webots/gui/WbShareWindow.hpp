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

#ifndef WB_SHARE_WINDOW_HPP
#define WB_SHARE_WINDOW_HPP

//
// Description: Share dialog for the Share button.
//

#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <core/WbConfig.h>

class WB_LIB_EXPORT WbShareWindow : public QDialog {
  Q_OBJECT
public:
  explicit WbShareWindow(QWidget *parent = NULL);
  virtual ~WbShareWindow() {}
};

class WB_LIB_EXPORT WbLinkWindow : public QDialog {
  Q_OBJECT
public:
  explicit WbLinkWindow(QWidget *parent = NULL);
  virtual ~WbLinkWindow() {}

  void setUploadUrl(const QString &url);

private:
  QGroupBox *mGroupBoxLink;
  QString mUrl;

  void reject() override;
  void openUrl();
};

#endif
