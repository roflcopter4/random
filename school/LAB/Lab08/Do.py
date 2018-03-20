#!/usr/bin/env python3

import sys
import getopt

FILE = 'input.txt'

##############################################################################
# Classes


class Page:
    def __init__(self, data):
        self.data = data
        self.students = [i[0] for i in data]
        self.grades = [i[1] for i in data]
        self.maximum = max(self.grades)
        self.minimum = min(self.grades)
        self.mean = sum(self.grades) / len(self.grades)

        self.page = self.makepage()

    def makepage(self):

        return

    #
    def nop(self):
        pass


##############################################################################
# Functions


def main():
    data = get_data()
    data = sorted(data, key=lambda x: data[0])
    page = Page(data)


def get_mean_median(grades):
    pass


def get_data():
    try:
        with open(FILE, 'r') as fp:
            return [i.rstrip() for i in fp]
    except IOError as e:
        print(e)
        sys.exit()


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
