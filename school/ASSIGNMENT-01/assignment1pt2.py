#!/usr/bin/env python3

import sys

# Takes either a filename or info from stdin as input. Alternatively a filename
# can be given interactively for opening.

# Even though I happen to know that the ascii value of 'A' is 65, it feels less
# 'evil global literals'-ish to calculate it.
ascii = [chr(c) for c in range(0, 128)]
A_INDEX = ascii.index('A')
#define A_INDEX 65


# ==============================================================================
# Functions
# ==============================================================================


def prompt_for_file():
    print("No input file specified. Falling back to a prompt. Leave blank to",
          "exit.")
    filename = input("Enter file name:  ")
    if len(filename) > 0:
        return filename
    else:
        sys.exit()


def get_data(filename):
    """Opens and reads a named file if it exists, sends the data to be parsed,
    and returns the results.
    """
    try:
        with open(filename, 'r') as fp:
            return parse_data(fp.readlines())
    except IOError:
        print("ERROR: Cannot read file '%s'. I didn't bother with" % filename,
              "recovery code, so this is fatal.")
        sys.exit(1)


def parse_data(raw_data):
    """Splits a list of strings in two, returning an array of lists composed of
    a string and an integer if the data is valid, or a None object otherwise.
    """
    data = []
    for line in raw_data:
        if line[0] != '\n':  # Filter blank lines. \n are removed by int().
            crypt = line.split()
            crlen = len(crypt)
            if crlen == 2:
                crypt[1] = int(crypt[1])
                data.append(crypt)

            # Only add non-empty lines to be marked as missing a key later.
            elif crlen == 1 and len(crypt[0]) > 0:
                data.append(None)
    return data


def decrypt(cypher, key):
    """Converts the cypher to an ascii bytearray then loops through it. Brings
    the ascii value of the character to within the range 0-25, adds the key,
    and performs moduly 26 to 'wrap' around to the start of the alphabet, then
    adds back the value subtracted earlier. Fairly standard kind of thing for
    which I lack a term, so instead I typed all this.
    Returns the entire decoded string converted back to a string object.
    """
    cypher_b = bytearray(cypher, 'ascii')
    for i in range(len(cypher_b)):
        cypher_b[i] = ((cypher_b[i] - A_INDEX + key) % 26) + A_INDEX
    return cypher_b.decode()


# ==============================================================================
# MAIN
# ==============================================================================


# Programs that refuse to take input on a command line are evil.
if len(sys.argv) > 1:
    data = get_data(sys.argv[1])
elif not sys.stdin.isatty():
    data = parse_data(sys.stdin.readlines())
else:
    data = get_data(prompt_for_file())

for crypt in data:
    if crypt is not None:  # Cyphers lacking keys were returned as None objects
        print(decrypt(*crypt))
    else:
        print("ERROR: Key not found.")
