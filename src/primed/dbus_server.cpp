#include <iostream>

#include "dbus_server.hpp"
#include "common/config.h"

DBusServer::DBusServer(DBus::Connection &connection) : DBus::ObjectAdaptor(connection, DBUS_SERVER_PATH) {

}

void DBusServer::hookXStarting(const uint32_t& pid) {

}

uint32_t DBusServer::hookLibglLoad(const uint32_t& pid, const std::string& dri_prime) {
    return 3;
}

void DBusServer::hookLibglUnload(const uint32_t& pid) {

}

void DBusServer::hookSystemSuspend() {

}

void DBusServer::hookSystemResume() {

}

std::string DBusServer::getStatus() {
    std::cout << "getStatus";
    return "hello";
}

std::map< uint32_t, std::vector< uint32_t > > DBusServer::getClients() {
    std::map<uint32_t, std::vector<uint32_t> > res;
    res[32].push_back(166);
    res[2].push_back(16);
    return res;
}

void DBusServer::setPower(const uint32_t& value) {

}

void DBusServer::reloadSettings() {

}
