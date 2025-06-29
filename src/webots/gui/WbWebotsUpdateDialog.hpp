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

#ifndef WB_WEBOTS_UPDATE_DIALOG_HPP
#define WB_WEBOTS_UPDATE_DIALOG_HPP

//
// Description: dialog allows to check if there is any Webots update
//

#include <QtWidgets/QDialog>

#include <core/WbConfig.h>

class QLabel;

class WB_LIB_EXPORT WbWebotsUpdateDialog : public QDialog {
  Q_OBJECT

public:
  explicit WbWebotsUpdateDialog(bool displayCheckBox, QWidget *parent = NULL);
  virtual ~WbWebotsUpdateDialog();

private slots:
  void updateLabel();
  void updatePreferences(bool doNotDisplayDialog);

private:
  QLabel *mLabel;
};

#endif
