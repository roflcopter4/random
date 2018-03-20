#!/usr/bin/env python3
# A Stack implementation

START = 'www.cs.ualberta.ca'

##############################################################################
# CLASSES


class Stack:

    def __init__(self):
        self.__items = []

    def push(self, item):
        self.__items.append(item)

    def pop(self):
        return self.__items.pop()

    def peek(self):
        return self.__items[len(self.__items)-1]

    def is_empty(self):
        return len(self.__items) == 0

    def size(self):
        return len(self.__items)

    def clear(self):
        self.__items = []

    def debug_print(self):
        print(self.__items)

##############################################################################
# FUNCTIONS


def check_pop_push(stFrom, stTo, act, cur):
    if stFrom.is_empty():
        print("Action '%s' is invalid" % act)
        return cur
    else:
        stTo.push(cur)
        cur = stFrom.pop()
        return cur


##############################################################################
# MAIN

def main():
    backward = Stack()
    forward = Stack()
    prev_action = '='
    current = START

    while True:
        print("\n[%s]" % current)
        action = input()
        if len(action) == 0:
            continue
        action = action[0]

        if action == '<':
            current = check_pop_push(backward, forward, action, current)
        elif action == '>':
            current = check_pop_push(forward, backward, action, current)
        elif action == '=':
            buf = input()
            if len(buf) > 0:
                backward.push(current)
                current = buf
                if prev_action == '<':
                    forward.clear()
        else:
            print("Invalid action '%s'", action)

        prev_action = action


main()
