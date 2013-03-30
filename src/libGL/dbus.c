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
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    DBusPendingCall* pending;

    // initialiset the errors
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Connection Error (%s)\n", err.message);
        dbus_error_free(&err);

        return -1;
    }

    // create a new method call and check for errors
    msg = dbus_message_new_method_call(DBUS_SERVER_NAME, DBUS_SERVER_PATH,
            DBUS_SERVER_NAME, "hookLibglLoad");

    if (NULL == msg) {
        fprintf(stderr, "Message Null\n");
        return -1;
    }

    // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &pid)) {
        fprintf(stderr, "Out Of Memory!\n");
        return -1;
    }

    // send message and get a handle for a reply
    if (!dbus_connection_send_with_reply(conn, msg, &pending, -1)) { // -1 is default timeout
        fprintf(stderr, "Out Of Memory!\n");
        return -1;
    }
    if (NULL == pending) {
        fprintf(stderr, "Pending Call Null\n");
        return -1;
    }
    dbus_connection_flush(conn);

    printf("Request Sent\n");

    // free message
    dbus_message_unref(msg);

    // block until we recieve a reply
    dbus_pending_call_block(pending);

    // get the reply message
    msg = dbus_pending_call_steal_reply(pending);
    if (NULL == msg) {
        fprintf(stderr, "Reply Null\n");
        return -1;
    }
    // free the pending message handle
    dbus_pending_call_unref(pending);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message has no arguments!\n");
    else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not UINT32!\n");
    else
        dbus_message_iter_get_basic(&args, ret);


    printf("Got Reply: %d\n", *ret);

    // free reply and close connection
    dbus_message_unref(msg);
    //dbus_connection_close(conn);
    return 0;
}

