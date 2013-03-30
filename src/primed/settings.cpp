/*
 * settings.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "settings.hpp"

#include <boost/property_tree/info_parser.hpp>

Settings::Settings() {
}

Settings::~Settings() {
}

void Settings::load(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    boost::property_tree::read_info(filename, m_props);
}
