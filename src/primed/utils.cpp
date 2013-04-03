/*
 * utils.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: nsl
 */

#include "utils.hpp"

#include <iostream>
#include <fstream>

#include <boost/format.hpp>
#include <boost/algorithm/string/split.hpp>

namespace utils {

EnvDict getEnvDict(pid_t pid) {
    EnvDict res;
    std::string fname = (boost::format("/proc/%d/environ") % pid).str();

    std::ifstream in(fname);
    std::string text((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());
    in.close();

    std::vector<std::string> out;
    boost::split(out, text, [](char c) {return c == '\0';});
    for (auto &x : out) {
        size_t pos = x.find('=');

        std::string key = x.substr(0, pos);
        std::string value = pos == std::string::npos ? "" : x.substr(pos + 1);

        if (key != "")
            res[key] = value;
    }

    return res;
}

} /* namespace utils */

