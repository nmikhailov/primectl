/*
 * process_tracker.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "process_tracker.hpp"

#include <signal.h>

#include <thread>

ProcessTracker::ProcessTracker(Settings& prop) :
        m_settings(prop) {
}

ProcessTracker::~ProcessTracker() {
}

void ProcessTracker::startTracking(pid_t pid) {
    std::thread t([pid, this] {
        while(kill(pid, 0) == 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        this->m_exit_signal(pid);
    });
    t.detach();
}

void ProcessTracker::connectForkSignal(ForkSignal::slot_type fn, int priority) {
    m_fork_signal.connect(priority, fn);
}

void ProcessTracker::connectExitSignal(ExitSignal::slot_type fn, int priority) {
    m_exit_signal.connect(priority, fn);
}
