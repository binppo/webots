/*
 * Copyright 1996-2019 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**********************************************************************************/
/* Description:  Webots C programming interface for the Display node              */
/**********************************************************************************/

#ifndef WB_DISPLAY_H
#define WB_DISPLAY_H

#define WB_USING_C_API
#include <webots/types.h>

#ifdef __cplusplus
//extern "C" {
#endif

CONTROLLER_EXPORT extern int wb_display_get_width(WbRobotContext *context, WbDeviceTag tag);
CONTROLLER_EXPORT extern int wb_display_get_height(WbRobotContext *context, WbDeviceTag tag);

// drawing properties
CONTROLLER_EXPORT extern void wb_display_set_color(WbRobotContext *context, WbDeviceTag tag, int color);
CONTROLLER_EXPORT extern void wb_display_set_alpha(WbRobotContext *context, WbDeviceTag tag, double alpha);
CONTROLLER_EXPORT extern void wb_display_set_opacity(WbRobotContext *context, WbDeviceTag tag, double opacity);
CONTROLLER_EXPORT extern void wb_display_set_font(WbRobotContext *context, WbDeviceTag tag, const char *font, int size, bool anti_aliasing);

CONTROLLER_EXPORT extern void wb_display_attach_camera(WbRobotContext *context, WbDeviceTag tag, WbDeviceTag camera_tag);
CONTROLLER_EXPORT extern void wb_display_detach_camera(WbRobotContext *context, WbDeviceTag tag);

// draw primitive
CONTROLLER_EXPORT extern void wb_display_draw_pixel(WbRobotContext *context, WbDeviceTag tag, int x, int y);
CONTROLLER_EXPORT extern void wb_display_draw_line(WbRobotContext *context, WbDeviceTag tag, int x1, int y1, int x2, int y2);
CONTROLLER_EXPORT extern void wb_display_draw_rectangle(WbRobotContext *context, WbDeviceTag tag, int x, int y, int width, int height);
CONTROLLER_EXPORT extern void wb_display_draw_oval(WbRobotContext *context, WbDeviceTag tag, int cx, int cy, int a, int b);
CONTROLLER_EXPORT extern void wb_display_draw_polygon(WbRobotContext *context, WbDeviceTag tag, const int *x, const int *y, int size);
CONTROLLER_EXPORT extern void wb_display_draw_text(WbRobotContext *context, WbDeviceTag tag, const char *text, int x, int y);
CONTROLLER_EXPORT extern void wb_display_fill_rectangle(WbRobotContext *context, WbDeviceTag tag, int x, int y, int width, int height);
CONTROLLER_EXPORT extern void wb_display_fill_oval(WbRobotContext *context, WbDeviceTag tag, int cx, int cy, int a, int b);
CONTROLLER_EXPORT extern void wb_display_fill_polygon(WbRobotContext *context, WbDeviceTag tag, const int *x, const int *y, int size);

// WbImageRef handle functions
//#define WB_IMAGE_RGB 3
//#define WB_IMAGE_RGBA 4
//#define WB_IMAGE_ARGB 5
//#define WB_IMAGE_BGRA 6

CONTROLLER_EXPORT extern WbImageRef wb_display_image_new(WbRobotContext *context, WbDeviceTag tag, int width, int height, const void *data, int format);
CONTROLLER_EXPORT extern WbImageRef wb_display_image_copy(WbRobotContext *context, WbDeviceTag tag, int x, int y, int width, int height);
CONTROLLER_EXPORT extern WbImageRef wb_display_image_load(WbRobotContext *context, WbDeviceTag tag, const char *filename);
CONTROLLER_EXPORT extern void wb_display_image_delete(WbRobotContext *context, WbDeviceTag tag, WbImageRef ir);
CONTROLLER_EXPORT extern void wb_display_image_paste(WbRobotContext *context, WbDeviceTag tag, WbImageRef ir, int x, int y, bool blend);
CONTROLLER_EXPORT extern void wb_display_image_save(WbRobotContext *context, WbDeviceTag tag, WbImageRef ir, const char *filename);

#ifdef __cplusplus
//}
#endif

#endif /* WB_DISPLAY_H */
