/*
 * xserverctl.hpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#ifndef XSERVERCTL_HPP_
#define XSERVERCTL_HPP_

#include "settings.hpp"

class XServerCtl {
public:
    XServerCtl(Settings &prop);
    virtual ~XServerCtl();

    void initializeOffload(pid_t client_id);

protected:
    Settings &m_settings;
};

#endif /* XSERVERCTL_HPP_ */
