/*
 * gpu_switch.hpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#ifndef GPU_SWITCH_HPP_
#define GPU_SWITCH_HPP_

#include "settings.hpp"

class GPUSwitch {
public:
    GPUSwitch(Settings &prop);
    virtual ~GPUSwitch();

    void setSecondaryEnabled(bool enabled) const;
    bool enabled() const;

protected:
    std::string getDebugfsMountPoint() const;

    Settings &m_settings;
};

#endif /* GPU_SWITCH_HPP_ */
