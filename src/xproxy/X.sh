#!/bin/sh
pid=$(readlink /proc/self)
dbus-send --session --type=method_call --dest=org.dri.PrimeCtl /org/dri/PrimeCtl org.dri.PrimeCtl.hookXStarting uint32:$pid
exec X
