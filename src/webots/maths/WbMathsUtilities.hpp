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

#ifndef WB_MATHS_UTILITIES_HPP
#define WB_MATHS_UTILITIES_HPP

#include <math.h>

#include <QtCore/QVector>

#include <core/WbConfig.h>

class WbPolygon;
class WbVector2;
class WbVector3;

namespace WbMathsUtilities {

  static const double EPSILON = 0.0001;

  enum { X, Y, Z };

  WB_LIB_EXPORT bool isPowerOf2(unsigned int n);
  WB_LIB_EXPORT unsigned int nextPowerOf2(unsigned int n);
  WB_LIB_EXPORT int round(double n);
  WB_LIB_EXPORT inline double discretize(double value, double resolution) {
    return ((int)(value / resolution + 0.5)) * resolution;
  }
  WB_LIB_EXPORT inline double discretize(float value, float resolution) {
    return ((int)(value / resolution + 0.5f)) * ((double)resolution);
  }

  // performs two Graham scan and returns the indices of points in the convex hull
  WB_LIB_EXPORT int twoStepsConvexHull(const QVector<WbVector2> &points, QVector<int> &hullIndices);
  WB_LIB_EXPORT int convexHull(const QVector<WbVector2> &points, QVector<int> &hullIndices);
  // builds a direct orthonormal basis (b[X], b[Y] = vY.normalized(), b[Z])
  WB_LIB_EXPORT void orthoBasis(const WbVector3 &vY, WbVector3 b[3]);
  WB_LIB_EXPORT bool isConvex(const WbPolygon &p);

  // Angles
  // find alpha such that |alpha - lastSpot| <= pi and alpha = angle mod 2 * pi
  WB_LIB_EXPORT inline double normalizeAngle(double angle, double lastSpot);
  WB_LIB_EXPORT bool isZeroAngle(double angle);
  // clamps angles in the -pi..pi range and possibly swap clamped angles to comply with min <= max.
  WB_LIB_EXPORT void clampAngles(double &min, double &max);
  // Vectors
  WB_LIB_EXPORT bool isZeroVector3(const double *v);
  WB_LIB_EXPORT void printVector3(const char *str, const double *v);
  WB_LIB_EXPORT void printMatrix3x4(const char *str, const double *m);
  // Find rational approximation of given real number
  // returns false if no approximation is found
  WB_LIB_EXPORT bool computeRationalApproximation(double value, int maxDenominator, int &numerator, int &denominator);
  // This will clamp value between -1 and 1 to avoid NaN generation
  WB_LIB_EXPORT inline double clampedAcos(double value);
  // This will clamp value between -1 and 1 to avoid indefinite value being generated
  WB_LIB_EXPORT inline double clampedAsin(double value);
};  // namespace WbMathsUtilities

// Normalize angle to be within +/-pi of lastSpot.
inline double WbMathsUtilities::normalizeAngle(double angle, double lastSpot = 0.0) {
  static const double INV_TWO_PI = 0.5 / M_PI;
  double d = angle - lastSpot;
  d -= floor(d * INV_TWO_PI) * 2.0 * M_PI;
  if (d > M_PI)
    d -= 2.0 * M_PI;
  return d + lastSpot;
}

// Make sure that the value is within the valid range before calling acos
// If not then the behavior is similar to a clamped value.
inline double WbMathsUtilities::clampedAcos(double value) {
  assert((fabs(value) < 1.0 + EPSILON) && "Value passed to clampedAcos out of range.");
  if (value >= 1.0)
    return 0.0;
  if (value <= -1.0)
    return M_PI;
  return acos(value);
}

// Make sure that the value is within the valid range before calling asin
// If not then the behavior is similar to a clamped value.
inline double WbMathsUtilities::clampedAsin(double value) {
  assert((fabs(value) < 1.0 + EPSILON) && "Value passed to clampedAsin out of range.");
  if (value >= 1.0)
    return M_PI / 2;
  if (value <= -1.0)
    return -M_PI / 2;
  return asin(value);
}

#endif  // WB_MATHS_UTILITIES_HPP
