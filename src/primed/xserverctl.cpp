/*
 * xserverctl.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "xserverctl.hpp"

#include "utils.hpp"

XServerCtl::XServerCtl(Settings& prop) :
        m_settings(prop) {
}

XServerCtl::~XServerCtl() {
}

void XServerCtl::initializeOffload(pid_t client_id) {
    utils::getEnvDict(client_id);
}
