#!/usr/bin/env python3

import sys

##############################################################################
# FUNCTIONS
##############################################################################


def read_file(filename):
    try:
        with open(filename, 'r') as f:
            return [buf.split(':') for buf in f.readlines()]
    except IOError:
        print("ERROR: Cannot read file '%s'. I didn't bother with" % filename,
              "recovery code, so this is fatal.")
        sys.exit(1)


def parse_data(data):
    acc_dict = {}
    for account in data:
        try:
            name = account[0]
            value = float(account[1])
            acc_dict.update({name: value})
        except ValueError:
            print("Illegal value for %s, not adding account to record."
                  % account[0])
    return acc_dict


##############################################################################
# MAIN
##############################################################################


data = parse_data(read_file('accounts.txt'))
print("Welcome to whatever this program is.")

while(True):
    try:
        in_account = input('\nEnter acccount name, or Stop or ^c to exit: ')
    except KeyboardInterrupt:
        print()
        break
    if in_account == 'Stop':
        break
    try:
        balance = data[in_account]
    except KeyError:
        print("ERROR: No such account '%s'" % in_account)
        continue

    print("%s: %.2f" % (in_account, balance))
    in_value = input("Enter transaction ammount: ")

    try:
        in_value = float(in_value)
    except ValueError:
        print("ERROR: Invalid entry")
        continue

    balance += in_value
    data[in_account] = balance

    print("New balance for account %s: %.2f" % (in_account, balance))
