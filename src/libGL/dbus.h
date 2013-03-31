/*
 * dbus.h
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#ifndef DBUS_H_
#define DBUS_H_

#include <unistd.h>

#define BUS_ERROR_CONNECT  1
#define BUS_ERROR_SEND     2
#define BUS_ERROR_RECIVE   3

int dbus_send_hook_load(pid_t pid, int *ret);


#endif /* DBUS_H_ */
