#ifndef DBUS_SERVER_HPP__
#define DBUS_SERVER_HPP__

#include <dbus-c++/dbus.h>
#include <cassert>
#include <thread>
#include <mutex>
#include <unordered_set>

#include "dbus_adaptor.hpp"

class DBusServer:
    public org::dri::PrimeCtl_adaptor,
    public DBus::IntrospectableAdaptor,
    public DBus::ObjectAdaptor {
public:
    DBusServer(DBus::Connection &connection);

    void hookXStarting(const uint32_t& pid);
    uint32_t hookLibglLoad(const uint32_t& pid, const std::string& dri_prime);
    void hookLibglUnload(const uint32_t& pid);
    void hookSystemSuspend();
    void hookSystemResume();
    std::string getStatus();
    std::map< uint32_t, std::vector<uint32_t>> getClients();
    void setPower(const uint32_t& value);
    void reloadSettings();

protected:
    std::unordered_set<pid_t> m_clients;

    std::mutex m_threads_mutex, m_clients_mutex;
    std::vector<std::thread> m_threads;
};


#endif // DBUS_SERVER_HPP__
