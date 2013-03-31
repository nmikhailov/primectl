/*
 * dbus.c
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "dbus.h"

#include <dbus/dbus.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#include "common/config.h"

int dbus_send_hook_load(pid_t pid, int *ret) {
    DBusMessage *msg, *msg_ret;
    DBusMessageIter args;
    DBusConnection *conn;
    DBusError err;
    int res = 0;

    dbus_error_init(&err);

    // Connect to dbus
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        dbus_error_free(&err);

        res = BUS_ERROR_CONNECT;
        goto error_connect;
    }

    // Create method call
    msg = dbus_message_new_method_call(DBUS_SERVER_NAME, DBUS_SERVER_PATH,
            DBUS_SERVER_NAME, "hookLibglLoad");

    if (msg == NULL ) {
        res = BUS_ERROR_CONNECT;
        goto error_connect;
    }

    // Add arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &pid)) {
        res = BUS_ERROR_CONNECT;
        goto error_msg;
    }

    // Send message
    msg_ret = dbus_connection_send_with_reply_and_block(conn, msg, 5 * 1000, &err);

    if (dbus_error_is_set(&err) || msg_ret == NULL) {
        res = BUS_ERROR_SEND;
        goto error_msg;
    }

    // Read parameters
    if (!dbus_message_iter_init(msg_ret, &args)) {
        res = BUS_ERROR_RECIVE;
        goto error_ret;
    }

    if (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_INT32) {
        res = BUS_ERROR_RECIVE;
        goto error_ret;
    }

    dbus_message_iter_get_basic(&args, ret);

error_ret:
    dbus_message_unref(msg_ret);
error_msg:
    dbus_message_unref(msg);
error_connect:
    dbus_error_free(&err);

    return res;
}
