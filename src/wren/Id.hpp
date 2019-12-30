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

#ifndef ID_HPP
#define ID_HPP

#include "Constants.hpp"

namespace wren {

  // Uniquely identifies a resource when sorting Renderables by their required state.
  struct Id {
  public:
    explicit Id(uint16_t id);

    uint16_t id() const { return mId; }

  protected:
    uint16_t mId;
  };

  struct IdPhongMaterial : public Id {
  public:
    static void resetCounter() { IdPhongMaterial::cCounter = 1; }

    IdPhongMaterial();

  private:
    static uint16_t cCounter;
  };

  struct IdPbrMaterial : public Id {
  public:
    static void resetCounter() { IdPbrMaterial::cCounter = 1; }

    IdPbrMaterial();

  private:
    static uint16_t cCounter;
  };

  struct IdMesh : public Id {
  public:
    static void resetCounter() { IdMesh::cCounter = 1; }

    IdMesh();

  private:
    static uint16_t cCounter;
  };

}  // namespace wren

#endif  // ID_HPP
