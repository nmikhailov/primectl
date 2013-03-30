/*
 * client_pool.hpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#ifndef CLIENT_POOL_HPP_
#define CLIENT_POOL_HPP_

#include <mutex>
#include <set>
#include <map>

#include "settings.hpp"

class ClientPool {
public:
    ClientPool(Settings &prop);
    virtual ~ClientPool();

    void addClient(pid_t pid);
    void removeClient(pid_t pid);

    std::vector<uint32_t> getClients() const;
    size_t size() const;

protected:
    Settings &m_settings;
    mutable std::mutex m_mutex;

    std::set<pid_t> m_clients;
};

#endif /* CLIENT_POOL_HPP_ */
