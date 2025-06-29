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

#ifndef WB_DRAG_OVERLAY_EVENT_HPP
#define WB_DRAG_OVERLAY_EVENT_HPP

//
// Description: classes allowing to store data related with the device overlay dragging
//

#include "WbAbstractDragEvent.hpp"

#include <core/WbConfig.h>

class WbRenderingDevice;

// WbDragOverlayEvent abstract class
class WB_LIB_EXPORT WbDragOverlayEvent : public WbDragEvent {
public:
  enum DragOverlayType { TRANSLATE = 0, RESIZE };
  WbDragOverlayEvent(const QPoint &initialMousePosition, WbRenderingDevice *renderingDevice);
  virtual ~WbDragOverlayEvent() override{};
  virtual DragOverlayType type() = 0;
  void apply(const QPoint &currentMousePosition) override = 0;

protected:
  QPoint mInitialMousePosition;
  WbRenderingDevice *mRenderingDevice;
};

// WbDragTranslateOverlayEvent class: change the position of an overlay device
//                                    by dragging the mouse
class WB_LIB_EXPORT WbDragTranslateOverlayEvent : public WbDragOverlayEvent {
public:
  WbDragTranslateOverlayEvent(const QPoint &initialMousePosition, const QPoint &windowSize, WbRenderingDevice *renderingDevice);
  virtual ~WbDragTranslateOverlayEvent() override{};
  DragOverlayType type() override { return TRANSLATE; }
  void apply(const QPoint &currentMousePosition) override;

protected:
  QPoint mWindowSize;
  double mHalfWidth;
  double mHalfHeight;
};

// WbDragResizeOverlayEvent class: resize the overlay device
//                                 by dragging the mouse
class WB_LIB_EXPORT WbDragResizeOverlayEvent : public WbDragOverlayEvent {
public:
  WbDragResizeOverlayEvent(const QPoint &initialMousePosition, WbRenderingDevice *renderingDevice);
  virtual ~WbDragResizeOverlayEvent() override;
  DragOverlayType type() override { return RESIZE; }
  void apply(const QPoint &currentMousePosition) override;

protected:
  QPoint mInitialOverlaySize;
  double mTextureWidthInv;
  double mTextureHeightInv;
};

#endif
