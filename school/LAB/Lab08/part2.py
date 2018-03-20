#!/usr/bin/env python3

# import random

RANDRANGE = (0, 101)
GUESSMAX = 7

###############################################################################
# Classes


class Guesser:

    def __init__(self, a, b):
        self.guess_list = []
        self.low = a
        self.high = b
        self.last_guess = None

    def update(self, status):
        if status == '-':
            print(status)
            self.low = self.last_guess
        elif status == '+':
            print(status)
            self.high = self.last_guess

    def guess(self):
        self.last_guess = (self.high + self.low) // 2
        return self.last_guess


###############################################################################
# Functions


def main():
    numguesses = 0
    # num = random.randint(*RANDRANGE)
    num = int(input("Enter number to guess:  "))
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
