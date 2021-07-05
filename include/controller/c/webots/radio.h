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
/* Description:  Webots C programming interface for the Radio node                */
/**********************************************************************************/

#ifndef WB_RADIO_H
#define WB_RADIO_H

#define WB_USING_C_API
#include <webots/types.h>

#ifdef __cplusplus
//extern "C" {
#endif

typedef void *WbRadioMessage;
typedef void *WbRadioEvent;

CONTROLLER_EXPORT extern WbRadioMessage wb_radio_message_new(WbRobotContext *context, int length, const char *body, const char *destination);
CONTROLLER_EXPORT extern void wb_radio_message_delete(WbRadioMessage);
CONTROLLER_EXPORT extern const char *wb_radio_message_get_destination(WbRobotContext *context, WbRadioMessage);
CONTROLLER_EXPORT extern int wb_radio_message_get_length(WbRobotContext *context, WbRadioMessage);
CONTROLLER_EXPORT extern const char *wb_radio_message_get_body(WbRobotContext *context, WbRadioMessage);

CONTROLLER_EXPORT extern void wb_radio_enable(WbRobotContext *context, WbDeviceTag tag, int sampling_period);
CONTROLLER_EXPORT extern void wb_radio_disable(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern void wb_radio_set_address(WbRobotContext *context, WbDeviceTag tag, const char *address);
CONTROLLER_EXPORT extern const char *wb_radio_get_address(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern void wb_radio_set_frequency(WbRobotContext *context, WbDeviceTag tag, double hz);
CONTROLLER_EXPORT extern double wb_radio_get_frequency(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern void wb_radio_set_channel(WbRobotContext *context, WbDeviceTag tag, int channel);
CONTROLLER_EXPORT extern int wb_radio_get_channel(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern void wb_radio_set_bitrate(WbRobotContext *context, WbDeviceTag tag, int bits_per_second);
CONTROLLER_EXPORT extern int wb_radio_get_bitrate(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern void wb_radio_set_rx_sensitivity(WbRobotContext *context, WbDeviceTag tag, double dBm);
CONTROLLER_EXPORT extern double wb_radio_get_rx_sensitivity(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern void wb_radio_set_tx_power(WbRobotContext *context, WbDeviceTag tag, double dBm);
CONTROLLER_EXPORT extern double wb_radio_get_tx_power(WbRobotContext *context, WbDeviceTag tag);

CONTROLLER_EXPORT extern void wb_radio_set_callback(WbRobotContext *context, WbDeviceTag tag, void (*)(const WbRadioEvent));

CONTROLLER_EXPORT extern WbDeviceTag wb_radio_event_get_radio(WbRobotContext *context, const WbRadioEvent);
CONTROLLER_EXPORT extern char *wb_radio_event_get_data(WbRobotContext *context, const WbRadioEvent);
CONTROLLER_EXPORT extern int wb_radio_event_get_data_size(WbRobotContext *context, const WbRadioEvent);
CONTROLLER_EXPORT extern const char *wb_radio_event_get_emitter(WbRobotContext *context, const WbRadioEvent);
CONTROLLER_EXPORT extern double wb_radio_event_get_rssi(WbRobotContext *context, const WbRadioEvent);

CONTROLLER_EXPORT extern void wb_radio_send(WbRobotContext *context, WbDeviceTag tag, const WbRadioMessage, double delay);

#ifdef __cplusplus
//}
#endif

#endif /* WB_RADIO_H */
