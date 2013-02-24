#include <climits>
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>

#include "dbus_server.hpp"
#include "common/config.h"

DBus::BusDispatcher dispatcher;

void niam(int sig) {
    dispatcher.leave();
}

int main() {
    signal(SIGTERM, niam);
    signal(SIGINT, niam);

    DBus::default_dispatcher = &dispatcher;

    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name(DBUS_SERVER_NAME);

    DBusServer server(conn);

    dispatcher.enter();

    return 0;
}
