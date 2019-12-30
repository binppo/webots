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
extern "C" {
#endif

typedef void *WbRadioMessage;
typedef void *WbRadioEvent;

CONTROLLER_EXPORT WbRadioMessage wb_radio_message_new(int length, const char *body, const char *destination);
CONTROLLER_EXPORT void wb_radio_message_delete(WbRadioMessage);
CONTROLLER_EXPORT const char *wb_radio_message_get_destination(WbRadioMessage);
CONTROLLER_EXPORT int wb_radio_message_get_length(WbRadioMessage);
CONTROLLER_EXPORT const char *wb_radio_message_get_body(WbRadioMessage);

CONTROLLER_EXPORT void wb_radio_enable(WbDeviceTag tag, int sampling_period);
CONTROLLER_EXPORT void wb_radio_disable(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_radio_set_address(WbDeviceTag tag, const char *address);
CONTROLLER_EXPORT const char *wb_radio_get_address(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_radio_set_frequency(WbDeviceTag tag, double hz);
CONTROLLER_EXPORT double wb_radio_get_frequency(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_radio_set_channel(WbDeviceTag tag, int channel);
CONTROLLER_EXPORT int wb_radio_get_channel(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_radio_set_bitrate(WbDeviceTag tag, int bits_per_second);
CONTROLLER_EXPORT int wb_radio_get_bitrate(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_radio_set_rx_sensitivity(WbDeviceTag tag, double dBm);
CONTROLLER_EXPORT double wb_radio_get_rx_sensitivity(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_radio_set_tx_power(WbDeviceTag tag, double dBm);
CONTROLLER_EXPORT double wb_radio_get_tx_power(WbDeviceTag tag);

CONTROLLER_EXPORT void wb_radio_set_callback(WbDeviceTag tag, void (*)(const WbRadioEvent));

CONTROLLER_EXPORT WbDeviceTag wb_radio_event_get_radio(const WbRadioEvent);
CONTROLLER_EXPORT char *wb_radio_event_get_data(const WbRadioEvent);
CONTROLLER_EXPORT int wb_radio_event_get_data_size(const WbRadioEvent);
CONTROLLER_EXPORT char *wb_radio_event_get_emitter(const WbRadioEvent);
CONTROLLER_EXPORT double wb_radio_event_get_rssi(const WbRadioEvent);

CONTROLLER_EXPORT void wb_radio_send(WbDeviceTag tag, const WbRadioMessage, double delay);

#ifdef __cplusplus
}
#endif

#endif /* WB_RADIO_H */
