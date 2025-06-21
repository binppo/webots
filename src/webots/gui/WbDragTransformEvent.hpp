// Copyright 1996-2023 Cyberbotics Ltd.
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

#ifndef WB_DRAG_TRANSFORM_EVENT_HPP
#define WB_DRAG_TRANSFORM_EVENT_HPP

//
// Description: classes allowing to store data related with the transform mouse dragging
//

#include "WbAbstractDragEvent.hpp"

#include "WbAffinePlane.hpp"
#include "WbMatrix4.hpp"
#include "WbQuaternion.hpp"
#include "WbRay.hpp"
#include "WbVector2.hpp"
#include "WbVector3.hpp"

#include <QtCore/QSize>

#include <core/WbConfig.h>

class WbAbstractPose;
class WbWrenLabelOverlay;
class WbTranslateRotateManipulator;
class WbViewpoint;

// WbDragTransformEvent class (abstract) : change the position or the orientation of a transform node
class WB_LIB_EXPORT WbDragTransformEvent : public WbDragKinematicsEvent {
public:
  WbDragTransformEvent(WbViewpoint *viewpoint, WbAbstractPose *selectedTransform);
  virtual ~WbDragTransformEvent();
  void apply(const QPoint &currentMousePosition) override = 0;

protected:
  WbAbstractPose *mSelectedTransform;
};

// another abstract layer:
class WB_LIB_EXPORT WbTranslateEvent : public WbDragTransformEvent {
public:
  WbTranslateEvent(WbViewpoint *viewpoint, WbAbstractPose *selectedTransform);
  virtual ~WbTranslateEvent();
  void apply(const QPoint &currentMousePosition) override = 0;

protected:
  WbVector3 mScaleFromParents;
  const WbVector3 mInitialPosition;
  const WbVector3 mUpWorldVector;
  WbAffinePlane mDragPlane;
  WbRay mMouseRay;
  std::pair<bool, double> mIntersectionOutput;
};

// Implemented classes:

// WbDragHorizontalEvent class
class WB_LIB_EXPORT WbDragHorizontalEvent : public WbTranslateEvent {
public:
  WbDragHorizontalEvent(const QPoint &initialPosition, WbViewpoint *viewpoint, WbAbstractPose *selectedTransform);
  virtual ~WbDragHorizontalEvent();
  void apply(const QPoint &currentMousePosition) override;

private:
  WbVector3 mTranslationOffset;
  WbQuaternion mCoordinateTransform;
  bool mIsMouseRayValid;
};

// WbDragVerticalEvent class
class WB_LIB_EXPORT WbDragVerticalEvent : public WbTranslateEvent {
public:
  WbDragVerticalEvent(const QPoint &initialPosition, WbViewpoint *viewpoint, WbAbstractPose *selectedTransform);
  virtual ~WbDragVerticalEvent();
  void apply(const QPoint &currentMousePosition) override;

private:
  WbVector3 mNormal;
  WbVector3 mTranslationOffset;
};

// WbDragTranslateAlongAxisEvent class
class WB_LIB_EXPORT WbDragTranslateAlongAxisEvent : public WbDragTransformEvent {
  Q_OBJECT;

public:
  WbDragTranslateAlongAxisEvent(const QPoint &initialMousePosition, const QSize &widgetSize, WbViewpoint *viewpoint,
                                int handleNumber, WbAbstractPose *selectedTransform);
  virtual ~WbDragTranslateAlongAxisEvent();
  void apply(const QPoint &currentMousePosition) override;

protected:
  const WbVector3 mInitialMatterPosition;
  double mTranslationOffset;
  int mHandleNumber;
  WbTranslateRotateManipulator *mManipulator;
  WbWrenLabelOverlay *mTextOverlay;

  enum { X, Y, Z };
  int mCoordinate;
  double mMouseOffset;
  WbVector3 mHandleOffset;
  WbVector2 mWidgetSizeFactor;
  double mStepSize;
  WbVector2 mDirectionOnScreen;
};

class WB_LIB_EXPORT WbDragRotateAroundWorldVerticalAxisEvent : public WbDragTransformEvent {
  Q_OBJECT;

public:
  WbDragRotateAroundWorldVerticalAxisEvent(const QPoint &initialMousePosition, WbViewpoint *viewpoint,
                                           WbAbstractPose *selectedTransform);
  virtual ~WbDragRotateAroundWorldVerticalAxisEvent();
  void apply(const QPoint &currentMousePosition) override;

protected:
  const WbQuaternion mInitialQuaternionRotation;
  double mPreviousAngle;
  double mInitialMouseXPosition;
  const WbVector3 mUpWorldVector;
};

// WbDragRotateAroundAxisEvent class
class WB_LIB_EXPORT WbDragRotateAroundAxisEvent : public WbDragTransformEvent {
  Q_OBJECT;

public:
  WbDragRotateAroundAxisEvent(const QPoint &initialMousePosition, const QSize &widgetSize, WbViewpoint *viewpoint,
                              int handleNumber, WbAbstractPose *selectedTransform);
  virtual ~WbDragRotateAroundAxisEvent();
  void apply(const QPoint &currentMousePosition) override;

protected:
  WbTranslateRotateManipulator *mManipulator;
  WbWrenLabelOverlay *mTextOverlay;

  int mHandleNumber;
  int mCoordinate;
  const WbQuaternion mInitialQuaternionRotation;
  WbMatrix4 mInitialMatrix;
  WbVector3 mInitialPosition;
  double mZEye;
  double mStepSize;
  int mStepFractionNumerator;
  int mStepFractionDenominator;
  double mPreviousAngle;
  double mInitialAngle;
  WbVector2 mObjectScreenPosition;
  static const double RAD_TO_DEG;
};

#endif
