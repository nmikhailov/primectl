#include <iostream>
#include <functional>

#include "dbus_server.hpp"
#include "signal.h"
#include "common/config.h"

DBusServer::DBusServer(DBus::Connection &connection, Settings &prop) :
        DBus::ObjectAdaptor(connection, DBUS_SERVER_PATH),
        m_settings(prop),
        m_clients(prop),
        m_matcher(prop),
        m_pwrctl(prop),
        m_tracker(prop),
        m_xctl(prop) {

    namespace pl = std::placeholders;

    m_tracker.connectExitSignal(
            std::bind(&ClientPool::removeClient, &m_clients, pl::_1), 0);

    m_tracker.connectExitSignal([&](pid_t) {
        if (m_clients.size() == 0) {
            m_pwrctl.setSecondaryEnabled(false);
        }
    }, 1);

    m_tracker.connectForkSignal(
            std::bind(&ClientPool::addClient, &m_clients, pl::_1));
    m_tracker.connectForkSignal(
            std::bind(&ProcessTracker::startTracking, &m_tracker, pl::_1));
}

void DBusServer::hookXStarting(const uint32_t& pid) {
    m_pwrctl.setSecondaryEnabled(true);
}

uint32_t DBusServer::hookLibglLoad(const uint32_t& pid) {
    std::cout << "hookLibglLoad called. pid: " << pid << std::endl;

    if (m_matcher.matches(pid)) {
        m_pwrctl.setSecondaryEnabled(true);
        m_xctl.initializeOffload(pid);
        m_clients.addClient(pid);
        m_tracker.startTracking(pid);

        return 1; // ?
    } else {
        return -1;
    }
}

void DBusServer::hookSystemSuspend() {
    m_pwrctl.setSecondaryEnabled(true);
}

void DBusServer::hookSystemResume() {
    if (m_clients.size() == 0) {
        m_pwrctl.setSecondaryEnabled(false);
    }
}

std::string DBusServer::getStatus() {
    return "<STATUS_STRING>";
}

std::vector<uint32_t> DBusServer::getClients() {
    return m_clients.getClients();
}

void DBusServer::setPower(const uint32_t& value) {
    //m_switcher.setPower()
}

