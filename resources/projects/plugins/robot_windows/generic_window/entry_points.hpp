/*
 * Description:  Defines the entry point of the robot window library
 */

#ifndef ENTRY_POINTS_HPP
#define ENTRY_POINTS_HPP

struct WbRobotContext;
//extern "C" {
bool wbw_init(WbRobotContext*);
void wbw_cleanup();
void wbw_pre_update_gui();
void wbw_update_gui();
void wbw_read_sensors();
void wbw_write_actuators();
void wbw_show();
//}

#endif
