#ifndef DBUS_SERVER_HPP_
#define DBUS_SERVER_HPP_

#include <dbus-c++/dbus.h>
#include <cassert>
#include <thread>
#include <mutex>
#include <cstdint>
#include <string>
#include <map>
#include <unordered_set>

#include "dbus_adaptor.hpp"

#include "client_pool.hpp"
#include "client_matcher.hpp"
#include "gpu_switch.hpp"
#include "process_tracker.hpp"
#include "settings.hpp"
#include "xserverctl.hpp"

class DBusServer: public org::dri::PrimeCtl_adaptor,
        public DBus::IntrospectableAdaptor,
        public DBus::ObjectAdaptor {
public:
    DBusServer(DBus::Connection &connection, Settings &prop);

    void hookXStarting(const int32_t& pid) override;
    int32_t hookLibglLoad(const int32_t& pid) override;
    void hookSystemSuspend() override;
    void hookSystemResume() override;
    std::map<std::string, std::string> getStatus() override;
    std::vector<int32_t> getClients() override;
    void setPower(const int32_t& value) override;

protected:
    Settings &m_settings;

    ClientPool m_clients;
    ClientMatcher m_matcher;
    GPUSwitch m_pwrctl;
    ProcessTracker m_tracker;
    XServerCtl m_xctl;
};

#endif /* DBUS_SERVER_HPP_ */
