/*
 * settings.hpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <mutex>

// Thread safe property tree
class Settings {
public:
    Settings();
    virtual ~Settings();

    void load(const std::string &filename);

    template<class T>
    T get(const std::string &name, T def = T()) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_props.get(name, def);
    }

protected:
    boost::property_tree::ptree m_props;
    std::mutex m_mutex;
};

#endif /* SETTINGS_HPP_ */
