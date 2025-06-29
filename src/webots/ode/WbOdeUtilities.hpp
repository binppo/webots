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

#ifndef WB_ODE_UTILITIES_HPP
#define WB_ODE_UTILITIES_HPP

#include <core/WbConfig.h>

namespace WbOdeUtilities {
  WB_LIB_EXPORT void convertSpringAndDampingConstants(double spring, double damping, double timeStep, double &cfm, double &erp);
};

#endif
