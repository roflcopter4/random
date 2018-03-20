#!/usr/bin/env python3

import sys
import getopt
import re
from random import shuffle

FILENAME = "shuffledDeck.txt"

###############################################################################
# Classes


class Deck:

    def __init__(self, deck):
        self.deck = deck
        try:
            self.analyze_deck()  # Initial sanity check
        except AssertionError as err:
            self.handle_error(err)

    def analyze_deck(self, deck=None):
        """Checks a given 'deck' list for validity. If this is an instance of
        the Deck class this method can analyze its 'deck' attribute, or it can
        take one as a paramater. If the deck is not valid an angry warning will
        be displayed and the program euthenized. Specifically the deck list is
        required to contain:
            1) exactly 52 members (cards), all of which must be unique,
            2) only valid, correctly formatted playing cards (ie strings
               exactly of exactly 2 chars), and
            3) all of the required cards in a traditional paying card deck
               (excluding jokers).
        """
        deck = self.deck if deck is None else deck
        assert len(deck) == 52, "Deck is the wrong size."
        assert len(set(deck)) == 52, "Deck contains non-unique cards."

        buf = ''.join(deck)
        assert len(buf) == (52 * 2), \
            "All 'cards' in the deck must be exactly two characters long."

        # So there are 52 unique strings of size 2. Because they're unique the
        # only way they'll all match is if we have every required card.
        cre = re.compile("(?:[KQJA02-9][DCHS]){52}")
        assert cre.match(buf) is not None, "Deck is not complete."

    def handle_error(self, err):
        print("ERROR:   Deck sanity check failed. Aborting.", file=sys.stderr)
        print("Reason: ", err, file=sys.stderr)
        sys.exit()


###############################################################################
# Functions


def main(infile=None):
    deck_t = Deck(process_file(infile))
    shuffle(deck_t.deck)
    buf = ''
    for card in deck_t.deck:
        buf += card + ', '
    print(buf)


def process_file(infile=None):
    """Prompts the user for a filename (or optionally takes one as a
    paramater). If it is succesfully opened the file is slurped into a list,
    stripped of newlines, converted to uppercase only, and returned. On
    failure, the user is prompted again for either another filename, or given
    the option to terminate the program.
    """
    lst = None
    if infile is None:
        if 'FILENAME' in globals():
            infile = FILENAME
        else:
            infile = input("Enter filename:  ")
    while lst is None:
        try:
            with open(infile, 'r') as fp:
                lst = [i.rstrip().upper() for i in fp]
        except FileNotFoundError:
            print("File '%s' not found." % infile)
            infile = input("Please type another filename or hit enter to " +
                           "quit.\n > ")
            if infile in ('', '\n', '\r\n'):
                sys.exit()

        except IOError as err:
            print("Unexpected error:", err)
            sys.exit(1)

    return lst


###############################################################################
# Init


def get_options():
    kwargs = {}
    options = ['hf:', ['help', 'file=']]
    try:
        opts, args = getopt.getopt(sys.argv[1:], *options)
    except getopt.GetoptError as err:
        print(err)
        ShowUsage()
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt in ('h', "help"):
                ShowUsage(options)
            elif opt in ('f', "file"):
                kwargs['infile'] = str(optarg)
        return kwargs


def ShowUsage(options):
    print("""Usage: %s -[%s] --[%s]"""
          % (sys.argv[0], options[0], ', '.join(options[1])))
    sys.exit()


if __name__ == "__main__":
    main(**get_options())
