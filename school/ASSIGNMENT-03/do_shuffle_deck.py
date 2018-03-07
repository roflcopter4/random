#!/usr/bin/env python3

##########################################################################
# Creates a deck of 52 shuffles cards and stores in file shuffledDeck.txt
# each line of the file is a card. A card is 2 characters. The first is
# the rank, the second is the suit.
# Suits are: D, C, H, S
# Ranks are: K, Q, J, A, 2, 3, 4, 5, 6, 7, 8, 9, 0
#
# Program by Osmar Zaiane
# Version 1.0      2018-02-25
##########################################################################
from random import shuffle

suits = ["D", "C", "H", "S"]
ranks = ["K", "Q", "J", "A", "2", "3", "4", "5", "6", "7", "8", "9", "0"]

cards = []
for rank in ranks:
    for suit in suits:
        cards.append(rank + suit)

shuffle(cards)
buf = ""
for c in cards:
    buf += c + '\n'

print(buf[:-1])

# try:
#     with open("shuffledDeck.txt", "w") as cardFile:
#         for card in cards:
#             cardFile.write(card + "\n")
# except IOError as e:
#     print("I/O error: {0}: {1}".format(e.errno, e.strerror))
# except BaseException:
#     print("Unexpected error")
# else:
#     print("The following shuffled 52 card deck was saved in shuffledDeck.txt")
#     print(cards)
