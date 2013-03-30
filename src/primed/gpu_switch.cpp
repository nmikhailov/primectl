/*
 * gpu_switch.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "gpu_switch.hpp"

#include <iostream>
#include <fstream>

GPUSwitch::GPUSwitch(Settings& prop) :
        m_settings(prop) {
}

GPUSwitch::~GPUSwitch() {
}

void GPUSwitch::setSecondaryEnabled(bool enabled) const {
    std::cout << "GPUSwithch: Secondary GPU is "
            << (enabled ? "enabled" : "disabled") << std::endl;
}

bool GPUSwitch::enabled() const {
    return true;
}
