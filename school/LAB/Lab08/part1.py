#!/usr/bin/env python3

import random

###############################################################################
# Classes


###############################################################################
# Functions


def main():
    num = random.randint(0, 100)
    numguesses = 0

    print(num)
    while numguesses < 6:
        uin = int(input("Guess the number:  "))
        print("Sorry, try again.")
        if uin > num:
            print("Too high.")
        elif uin < num:
            print ("Too low.")
        else:
            break
        numguesses += 1
    print("You win!")


###############################################################################
# Init


if __name__ == "__main__":
    main()
