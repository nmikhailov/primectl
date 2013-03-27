#include <iostream>
#include <chrono>

#include "dbus_server.hpp"
#include "signal.h"
#include "common/config.h"

DBusServer::DBusServer(DBus::Connection &connection) : DBus::ObjectAdaptor(connection, DBUS_SERVER_PATH) {

}

void DBusServer::hookXStarting(const uint32_t& pid) {
    std::cout << "hookXStaring called. pid: " << pid << std::endl;
}

uint32_t DBusServer::hookLibglLoad(const uint32_t& pid, const std::string& dri_prime) {
    std::cout << "hookLibglLoad called. pid: " << pid << " prime env: " << dri_prime << std::endl;
    uint32_t res = -1;
    // res = ???
    //
    std::lock_guard<std::mutex> lock(m_clients_mutex);
    if (m_clients.empty()) {
        std::cout << "discrete ON" << std::endl;
    // echo ON > /sys/kernel/debug/vgaswitcheroo/switch
    }

    if (res > 0) { // Using discrete adapter
        std::lock_guard<std::mutex> clock(m_threads_mutex);
        m_clients.insert(pid);

        // Create thread & add to vector
        m_threads.emplace_back([this, pid] {
            while (true) {
                if (kill(pid, 0)) {
                    hookLibglUnload(pid);
                    break;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
    }

    return res;
}

void DBusServer::hookLibglUnload(const uint32_t& pid) {
    std::lock_guard<std::mutex> lock(m_clients_mutex);

    std::cout << "hookLibglUnload called. pid: " << pid << std::endl;
    if (m_clients.find(pid) == m_clients.end()) {
        std::cout << "already removed" << std::endl;
        return;
    }
    m_clients.erase(pid);

    if (m_clients.empty()) {
        // Turn OFF
        std::cout << "discrete OFF" << std::endl;
    }
}


void DBusServer::hookSystemSuspend() {
}

void DBusServer::hookSystemResume() {
}

std::string DBusServer::getStatus() {
    return "<STATUS_STRING>";
}

std::map<uint32_t, std::vector<uint32_t>> DBusServer::getClients() {
    std::lock_guard<std::mutex> lock(m_clients_mutex);

    std::map<uint32_t, std::vector<uint32_t>> res;
    if (!m_clients.empty()) {
        res[0].resize(m_clients.size());
        std::copy(m_clients.begin(), m_clients.end(), res[0].begin());
    }

    return res;
}

void DBusServer::setPower(const uint32_t& value) {

}

void DBusServer::reloadSettings() {

}
