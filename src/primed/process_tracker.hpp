/*
 * process_tracker.hpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#ifndef PROCESS_TRACKER_HPP_
#define PROCESS_TRACKER_HPP_

#include <boost/signals2.hpp>
#include "settings.hpp"

class ProcessTracker {
public:
    using ForkSignal = boost::signals2::signal<void(pid_t)>;
    using ExitSignal = boost::signals2::signal<void(pid_t)>;

    ProcessTracker(Settings &prop);
    virtual ~ProcessTracker();

    void startTracking(pid_t pid);

    void connectForkSignal(ForkSignal::slot_type fn, int priority = 10);
    void connectExitSignal(ExitSignal::slot_type fn, int priority = 10);

protected:
    Settings &m_settings;

    ForkSignal m_fork_signal;
    ExitSignal m_exit_signal;

};

#endif /* PROCESS_TRACKER_HPP_ */
