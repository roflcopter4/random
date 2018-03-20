#!/usr/bin/python3

###############################################################################
# Classes


class Node:

    def __init__(self, initData, initPrevious, initNext):
        """Constructs a new node and initializes it to contain the given object
        (initData) and links to the given next and previous nodes.
        """
        self.__data = initData
        self.__previous = initPrevious
        self.__next = initNext

        if not isinstance(self.__previous, EndNode):
            self.__previous.setNext(self)
        if not isinstance(self.__next, EndNode):
            self.__next.setPrevious(self)

    def getData(self):
        return self.__data

    def getNext(self):
        return self.__next

    def getPrevious(self):
        return self.__previous

    def setData(self, newData):
        self.__data = newData

    def setNext(self, newNext):
        self.__next = newNext

    def setPrevious(self, newPrevious):
        self.__previous = newPrevious

    def __repr__(self):
        sprev = '<' if not isinstance(self.__previous, EndNode) else '|'
        snext = '>' if not isinstance(self.__next, EndNode) else '|'
        buf = "[%s] - %s %s" % (self.__data, sprev, snext)
        return buf


class EndNode:
    def __init__(self):
        pass
