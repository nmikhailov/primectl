/*
 * client_matcher.hpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#ifndef CLIENT_MATCHER_HPP_
#define CLIENT_MATCHER_HPP_

#include "settings.hpp"

class ClientMatcher {
public:
    ClientMatcher(Settings &prop);
    virtual ~ClientMatcher();

    bool matches(pid_t client) const;

protected:
    Settings &m_settings;
};

#endif /* CLIENT_MATCHER_HPP_ */
