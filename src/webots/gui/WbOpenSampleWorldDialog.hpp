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

#ifndef WB_OPEN_SAMPLE_WORLD_DIALOG_HPP
#define WB_OPEN_SAMPLE_WORLD_DIALOG_HPP

#include <QtWidgets/QDialog>

#include <core/WbConfig.h>

class QDialogButtonBox;
class QItemSelection;
class QLineEdit;
class QStandardItemModel;
class QTreeView;

class WB_LIB_EXPORT WbOpenSampleWorldDialog : public QDialog {
  Q_OBJECT

public:
  explicit WbOpenSampleWorldDialog(QWidget *parent = NULL);
  virtual ~WbOpenSampleWorldDialog();

  QSize sizeHint() const override;

  const QString &selectedWorld() const { return mSelectedWorld; }

public slots:
  void accept() override;

private slots:
  void updateTree(const QString &reg = "");
  void handleDoubleClick(const QModelIndex &index);
  void handleSelectedItemChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
  QLineEdit *mFindLineEdit;
  QTreeView *mTreeView;
  QStandardItemModel *mModel;
  QDialogButtonBox *mButtonBox;
  QString mSelectedWorld;
};

#endif
