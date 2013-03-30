/*
 * client_pool.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "client_pool.hpp"

#include <thread>

ClientPool::ClientPool(Settings& prop) :
        m_settings(prop) {
}

ClientPool::~ClientPool() {
}

void ClientPool::addClient(pid_t pid) {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_clients.insert(pid);
}

void ClientPool::removeClient(pid_t pid) {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_clients.erase(pid);
}

std::vector<int32_t> ClientPool::getClients() const {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::vector<int32_t> res(m_clients.begin(), m_clients.end());

    return res;
}

size_t ClientPool::size() const {
    return m_clients.size();
}
