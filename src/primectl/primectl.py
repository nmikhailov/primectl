#!/usr/bin/env python3
# -*- encoding: utf-8 -*-
import dbus


class PrimeCtl(object):
    def __init__(self):
        self.bus = dbus.SessionBus()
        self.bus_obj = self.bus.get_object("org.dri.PrimeCtl",
                                           "/org/dri/PrimeCtl")
        self.primectl = dbus.Interface(self.bus_obj, "org.dri.PrimeCtl")

    def status(self):
        print(self.primectl.getStatus())
        clients = self.primectl.getClients()

        if len(clients) == 0:
            print("No clients running")

        for xpid, pids in clients.items():
            print("X Server %d, pid %d:" % (xpid, 0))
            for pid in pids:
                print("  Client pid %d:" % (pid))


def main():
    ctl = PrimeCtl()
    ctl.status()


if __name__ == '__main__':
    main()
