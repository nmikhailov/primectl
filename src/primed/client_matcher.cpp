/*
 * client_matcher.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "client_matcher.hpp"

ClientMatcher::ClientMatcher(Settings& prop) :
        m_settings(prop) {
}

ClientMatcher::~ClientMatcher() {
}

bool ClientMatcher::matches(pid_t client) const {
    return true;
}
