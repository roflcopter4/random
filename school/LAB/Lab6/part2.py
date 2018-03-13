#!/usr/bin/env python3

import sys
import getopt
# import random
# from timeit import repeat
# import subprocess
# import re


##############################################################################


class Queue:
    """Simple implementation of a bounded circular queue."""

    def __init__(self, capacity, name):
        assert capacity >= 2, "Max capacity for a queue must be at least 2."
        self.__capacity = capacity
        self.__items = [None]*capacity
        self.__count = 0
        self.__head = 0
        self.__tail = 0

        self.name = name

    def enqueue(self, item):
        """Places an item at the back of the queue."""
        assert self.__count < self.__capacity, \
            [self, 'full', "Queue is full!"]
        self.__items[self.__tail] = item
        self.__tail = (self.__tail + 1) % self.__capacity
        self.__count += 1

    def dequeue(self):
        """Returns and removes the item at the front of the queue."""
        assert self.__count > 0, (self, 'empty', "Queue is empty!")
        item = self.__items[self.__head]
        self.__items[self.__head] = None
        self.__head = (self.__head + 1) % self.__capacity
        self.__count -= 1
        return item

    def peek(self):
        """Returns the item at the front of the queue without removing it, or
        None if the the queue is empty (ie no exception is raised).
        """
        if self.__count > 0:
            return self.__items[self.__head]
        else:
            return None

    def get_size(self):
        return self.__count

    def is_full(self):
        return self.__count == (self.__capacity - 1)

    def is_empty(self):
        return self.__count == 0

    def clear(self):
        self.__items = [None for _ in range(self.__capacity)]
        self.__count = self.__head = self.__tail = 0

    def debug_get_items(self):
        """Returns the full item list as a python list. FOR DEBUGGING ONLY."""
        return self.__items

    def pdiagnostics(self):
        s = self  # Sleezy way to make the print fit into two lines < 79 chars.
        print("%s, H=%d, T=%d, (%d/%d)" %
              (str(s.__items), s.__head, s.__tail, s.__count, s.__capacity))

    def __str__(self):
        if self.__count:
            buf = '['
            for item in self.__items:
                if item is not None:
                    buf += str(item) + ", "
            return buf[:-2] + ']'
        else:
            return "[]"


# ----------------------------------------------------------------------------


class QueueEmptyError(BaseException):
    """Tried to dequeue from an empty queue."""


class QueueFullError(BaseException):
    """Tried to add to a full queue."""


##############################################################################
# Functions


def main():
    vips = Queue(3, "VIP")
    plebs = Queue(3, "Normal")
    print("""\
This program simulates a store queue. For each iteration of the main loop you
may specify one action - either to add a customer, serve a customer, or
murder all customers and exit. When adding a customer, their name and whether
they are VIP class either be entered separately or at once with a semicolon as
separation.\n""")

    store_open = True
    vips.enqueue('a')
    vips.enqueue('b')
    vips.enqueue('c')
    plebs.enqueue('d')
    while store_open:
        action = get_action()
        print()
        store_open = do_action(action, vips, plebs)
        if store_open:
            print("\nPeople in normal queue:", plebs.__str__())
            print("People in VIP queue:   ", vips.__str__())

            # DEBUG(vips, plebs)


def get_action():
    prompt = "Add, Serve, or Kill All Customers? (1,2,3) > "
    valid = ["add", "serve", "exit", "kill"]
    aliases = ['1', '2', '3']
    action = input(prompt)
    while action not in (*valid, *aliases):
        print("Unrecognzied action.")
        action = input(prompt)
    if action in aliases:
        action = valid[int(action) - 1]

    return action


def do_action(action, vips, plebs):
    if action == "add":
        name, isVIP = get_customer()
        try:
            if isVIP:
                vips.enqueue(name)
            else:
                plebs.enqueue(name)
        except AssertionError as e:
            print("Error: %s queue is full." % e.args[0][0].name)

        return True

    elif action == "serve":
        print(vips.get_size())
        if vips.get_size() > 0:
            print("%s customer '%s' has been served." %
                  (vips.name, vips.dequeue()))
        elif plebs.get_size() > 0:
            print("%s customer '%s' has been served." %
                  (plebs.name, plebs.dequeue()))
        else:
            print("Error: both queues are currently empty.")

        return True

    else:
        murder = False
        print()
        while vips.get_size():
            print("Snobby brat '%s' has been hanged." % vips.dequeue())
            murder = True
        while plebs.get_size():
            print("The filthy peasant '%s' has been hanged." % plebs.dequeue())
            murder = True

        if murder:
            print("All customers have been euthanized. Exiting.")
        else:
            print("Nothing to report. Exiting.")

        return False


def get_customer():
    prompt = "Enter customer name and (optionally) whether they are VIP.\n> "
    name = input(prompt)
    while not name:
        name = input(prompt)
    if name.count(';'):
        name, isVIP = name.split(';')
    else:
        prompt = "Is this customer VIP? y/n  "
        isVIP = input(prompt)
        while not isVIP:
            isVIP = input(prompt)

    name = name.rstrip('\n').rstrip(' ')
    isVIP = processVIP(isVIP)

    return name, isVIP


def processVIP(buf):
    buf = buf.lower().strip(' ').rstrip('\n')
    return buf in ('y', "yes", "true", "obviously")


def kill_all_customers(queue):
    queue.clear()


def DEBUG(vips, plebs):
    print("---------------------------------------")
    plebs.pdiagnostics()
    vips.pdiagnostics()
    print("---------------------------------------")


##############################################################################
# Init


# Getopt handling always looks so messy.
def get_options():
    """Process a few commandline options because I'm far, far too lazy to
    interactively type a filename and the number of cards for every test run.
    """
    kwargs = {}
    options = ["h", ["help"]]
    try:
        opts, args = getopt.getopt(sys.argv[1:], *options)
    except getopt.GetoptError as e:
        print(e)
        print()
        ShowUsage(options)
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt in ('h', "help"):
                ShowUsage(options)

        return kwargs


def optarg_to_int_or_die(opt, optarg):
    if optarg.isnumeric():
        return int(optarg)
    else:
        print("Option '%s' requires a numeric argument." % opt)
        sys.exit(1)


def ShowUsage(options):
    """Obligatory service."""
    print("""\
Usage: %s -[%s] --[%s]
-h --help         Show this help and exit."""
          % (sys.argv[0], options[0], ', '.join(options[1])))
    sys.exit()


if __name__ == "__main__":
    try:
        main(**get_options())
    except (EOFError, KeyboardInterrupt) as e:
        print("")
        sys.exit(3)
