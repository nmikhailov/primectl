/*
 * utils.hpp
 *
 *  Created on: Apr 2, 2013
 *      Author: nsl
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <map>

namespace utils {
using EnvDict = std::map<std::string, std::string>;

EnvDict getEnvDict(pid_t pid);

} /* namespace utils */
#endif /* UTILS_HPP_ */
