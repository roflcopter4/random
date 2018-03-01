#!/usr/bin/env python3
# CMPUT 175 Winter 2013 Lab 2 Exercise 2
# This program is used to calculate the worth of an automobile.

import sys


class Automobile:
    c_factor = {'red': 3, 'blue': 2, 'yellow': 2}

    # Constructor:
    def __init__(self, length, horsepower, color):
        self.__length = int(length)
        self.__horsepower = int(horsepower)
        self.__color = color
        try:
            factor = self.c_factor[color]
        except KeyError:
            factor = 1
        self.__worth = self.__horsepower * self.__length * factor * 10

    # Returns the length:
    def get_length(self):
        return self.__length

    # Returns the horsepower:
    def get_horsepower(self):
        return self.__horsepower

    # Returns the color:
    def get_color(self):
        return self.__color

    # Returns the worth:
    def get_worth(self):
        return self.__worth


def get_int(string):
    try:
        return int(string)
    except ValueError:
        print("ERROR: Invalid entry.")
        sys.exit()


# Main function, which asks the user for the length, horsepower, and color of
# an automobile, and will then print out the worth of that automobile:
def main():
    print("Please enter the paramaters for your new automobile.")
    length = input("Enter length in meters: ")
    try:
        length = int(length)
    except ValueError:
        print("ERROR: Invalid entry.")
        return -1
    horsepower = input("Enter horsepower: ")
    try:
        length = int(length)
    except ValueError:
        print("ERROR: Invalid entry.")
        return -1
    color = input("Enter color: ")
    car = Automobile(length, horsepower, color)

    print("The car's value would be: %.2f" % (car.get_worth()))


main()
