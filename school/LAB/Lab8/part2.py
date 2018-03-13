#!/usr/bin/env python3

import random

RANDRANGE = (0, 100)
GUESSMAX = 6

###############################################################################
# Classes


class Guesser:

    def __init__(self, a, b):
        self.guess_list = []
        self.window = [a, b]
        self.last_guess = None

    def update(self, status):
        if status == '-':
            print(status)
            self.window[0] = self.last_guess + 1
        elif status == '+':
            print(status)
            self.window[1] = self.last_guess - 1

    def guess(self):
        self.last_guess = random.randint(*self.window)
        return self.last_guess


###############################################################################
# Functions


def main():
    numguesses = 0
    # num = random.randint(*RANDRANGE)
    num = input("Enter number to guess:  ")
    guesser = Guesser(*RANDRANGE)

    # print(num)
    while numguesses < GUESSMAX:
        uin = guesser.guess()
        print("Guess was: %d" % uin)
        if uin > num:
            guesser.update("+")
        elif uin < num:
            guesser.update("-")
        else:
            print("The computer wins!")
            break
        numguesses += 1
    else:
        print("You lose, get off my property.")


###############################################################################
# Init


if __name__ == "__main__":
    main()
