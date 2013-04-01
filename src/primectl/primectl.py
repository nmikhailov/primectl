#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import dbus


class PrimeCtl(object):
    def __init__(self):
        self.bus = dbus.SessionBus()
        self.bus_obj = self.bus.get_object("org.dri.PrimeCtl",
                                           "/org/dri/PrimeCtl")
        self.primectl = dbus.Interface(self.bus_obj, "org.dri.PrimeCtl")

    def status(self):
        stat = self.primectl.getStatus()
        for val in stat:
            print(val, stat[val])

        clients = self.primectl.getClients()

        if len(clients) == 0:
            print("No clients running")
        else:
            print("{} clients:".format(len(clients)))

        for pid in clients:
            print("  pid %d:" % (pid))


def main():
    ctl = PrimeCtl()
    ctl.status()


if __name__ == '__main__':
    main()
