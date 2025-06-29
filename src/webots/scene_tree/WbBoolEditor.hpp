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

#ifndef WB_BOOL_EDITOR_HPP
#define WB_BOOL_EDITOR_HPP

//
// Description: editor for editing a WbSFBool or a WbMFBool item
//

#include "WbValueEditor.hpp"

#include <core/WbConfig.h>

class QCheckBox;

class WB_LIB_EXPORT WbBoolEditor : public WbValueEditor {
  Q_OBJECT

public:
  explicit WbBoolEditor(QWidget *parent = NULL);
  virtual ~WbBoolEditor() override;

  void recursiveBlockSignals(bool block) override;

  QWidget *lastEditorWidget() override;

public slots:
  void applyIfNeeded() override;

protected:
  void edit(bool copyOriginalValue) override;
  void resetFocus() override;

protected slots:
  void apply() override;

private:
  bool mBool;
  QCheckBox *mCheckBox;
  void takeKeyboardFocus() override;

  void updateText();
};

#endif
