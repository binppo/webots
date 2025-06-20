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

#ifndef WB_BILLBOARD_HPP
#define WB_BILLBOARD_HPP

#include "WbGroup.hpp"

#include <core/WbConfig.h>

class WB_LIB_EXPORT WbBillboard : public WbGroup {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbBillboard(WbTokenizer *tokenizer = NULL);
  WbBillboard(const WbBillboard &other);
  explicit WbBillboard(const WbNode &other);
  virtual ~WbBillboard() override;

  // reimplemented functions
  int nodeType() const override { return WB_NODE_BILLBOARD; }
  void postFinalize() override;
  void createWrenObjects() override;
  QList<const WbBaseNode *> findClosestDescendantNodesWithDedicatedWrenNode() const override {
    return QList<const WbBaseNode *>() << this;
  }

protected:
  void applyTranslationToWren();
  void applyRotationToWren();

private:
  WbBillboard &operator=(const WbBillboard &);  // non copyable
  WbNode *clone() const override { return new WbBillboard(*this); }

private slots:
  void updatePosition();
};

#endif
