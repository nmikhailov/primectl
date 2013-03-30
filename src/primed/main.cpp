#include <signal.h>
#include <unistd.h>

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <boost/property_tree/info_parser.hpp>

#include "common/config.h"

#include "dbus_server.hpp"
#include "settings.hpp"

DBus::BusDispatcher dispatcher;

void niam(int sig) {
    dispatcher.leave();
}

int main() {
    signal(SIGTERM, niam);
    signal(SIGINT, niam);

    Settings settings;
    try {
        settings.load(PRIMED_SETTINGS_FILENAME);
    } catch (boost::property_tree::info_parser::info_parser_error &e) {
        std::cout << "Can't open config for reading" << std::endl;
    }

    DBus::default_dispatcher = &dispatcher;

    DBus::Connection conn = DBus::Connection::SessionBus();
    conn.request_name(DBUS_SERVER_NAME);

    DBusServer server(conn, settings);

    dispatcher.enter();

    return 0;
}
