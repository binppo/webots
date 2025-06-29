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

#ifndef WB_INT_SPIN_BOX_HPP
#define WB_INT_SPIN_BOX_HPP

//
// Description: integer spin box using Webots cut/copy/paste and undo/redo handling
//

#include <QtWidgets/QSpinBox>

#include <core/WbConfig.h>

class WbClipboard;
class QKeyEvent;

class WB_LIB_EXPORT WbIntSpinBox : public QSpinBox {
  Q_OBJECT

public:
  explicit WbIntSpinBox(QWidget *parent = NULL);
  virtual ~WbIntSpinBox();

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  WbClipboard *mClipboard;

  // cut, copy, paste using Webots clipboard
  void cut();
  void copy() const;
  void paste();
};

#endif
