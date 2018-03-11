#!/usr/bin/python3

from d_linked_node import Node
from d_linked_node import EndNode


###############################################################################
# Classes


class d_linked_list:

    def __init__(self):
        self.__EndNode = EndNode()
        self.__head = self.__EndNode
        self.__tail = self.__EndNode
        self.__size = 0

    def search(self, item):
        current = self.__head
        while current != self.__EndNode and current.getData() != item:
            current = current.getNext()
        return (current == self.__EndNode)

    def index(self, item):
        index, node = self.__find_node_and_index(item)
        return index

    def add(self, item):
        tmp = Node(item, self.__EndNode, self.__head)
        if self.__head == self.__EndNode:
            self.__tail = tmp
        self.__head = tmp
        self.__size += 1

    def append(self, item):
        tmp = Node(item, self.__tail, self.__EndNode)
        if self.__tail == self.__EndNode:
            self.__head = tmp
        self.__tail = tmp
        self.__size += 1

    def insert(self, pos, item):
        if pos == 0:
            self.add(item)
        elif pos == (self.__size - 1):
            self.append(item)
        else:
            node = self.__getnode_at_index(pos)
            nprev = node.getPrevious()
            Node(item, nprev, node)
            self.__size += 1

    #
    def remove(self, item):
        index, node = self.__find_node_and_index(item)
        if index != -1:
            self.__do_remove(node)

    def pop(self, pos=None):
        node = self.__tail if pos is None else self.__getnode_at_index(pos)
        item = node.getData()
        self.__do_remove(node)
        return item

    #
    def search_larger(self, item):
        current = self.__head
        index = 0
        while current != self.__EndNode and current.getData() <= item:
            current = current.getNext()
            index += 1
        if current == self.__EndNode:
            index = -1
        return index

    def get_size(self):
        return self.__size

    def get_item(self, pos):
        return self.__getnode_at_index(pos).getData()

    #
    def __do_remove(self, node):
        if self.__size == 1:
            self.__head = self.__tail = self.__EndNode
            self.__size = 0
        else:
            if node == self.__head:
                self.__head = node.getNext()
                self.__head.setPrevious(self.__EndNode)
            elif node == self.__tail:
                self.__tail = node.getPrevious()
                self.__tail.setNext(self.__EndNode)
            else:
                nprev, nnext = (node.getPrevious(), node.getNext())
                nprev.setNext(nnext)
                nnext.setPrevious(nprev)

            node = None
            self.__size -= 1

    #
    def __getnode_at_index(self, pos):
        pos = int(pos)  # Will raise exception if pos can't be an integer.
        if pos < 0:
            pos = (self.__size) + pos
        assert (0 < pos < self.__size) or (pos == 0), "Out of bounds."
        assert self.__size > 0, "List is empty."

        # If pos is less than the size of the list divided by 2 (ie is in the)
        # first half of the list, start the search from the head, otherwise
        # start from the tail.
        if pos < ((self.__size - 1) // 2):
            x = 0
            current = self.__head
            while x != pos:
                current = current.getNext()
                x += 1
        else:
            x = self.__size - 1
            current = self.__tail
            while x != pos:
                current = current.getPrevious()
                x -= 1

        return current

    #
    def __find_node_and_index(self, item):
        current = self.__head
        index = 0
        while (current != self.__EndNode) and (current.getData() != item):
            current = current.getNext()
            index += 1

        if current == self.__EndNode:
            index = -1
        return index, current

    #
    def __str__(self):
        buf = []
        current = self.__head
        x = 0
        while x < self.__size:
            buf.append(str(current.getData()))
            current = current.getNext()
            x += 1
        return ' '.join(buf)


###############################################################################
# Functions


def dieifnot(a, b=None, c=None):
    fail_msg = "You have failed the test and shamed yourself and your country."
    tst_msg = "TEST: "

    if b is None and c is None:
        assert a, fail_msg
    else:
        if isinstance(a, int) and isinstance(b, int):
            print(tst_msg, "%d == %d" % (a, b))
        else:
            print(tst_msg, "'%s' == '%s'" % (a, b))
        assert a == b, fail_msg


def test():
    linked_list = d_linked_list()
    dieifnot(linked_list.get_size(), 0)

    # Test 1
    linked_list.add("World")
    linked_list.add("Hello")
    dieifnot(str(linked_list), "Hello World")
    dieifnot(linked_list.get_size(), 2)
    dieifnot(linked_list.get_item(0), "Hello")
    dieifnot(linked_list.get_item(1), "World")

    # Test 2
    is_pass = (linked_list.get_item(0) ==
               "Hello" and linked_list.get_size() == 2)
    dieifnot(is_pass is True)
    dieifnot(linked_list.pop(1), "World")
    dieifnot(linked_list.pop(), "Hello")
    dieifnot(linked_list.get_size(), 0)

    # Test 3
    int_list2 = d_linked_list()
    for i in range(0, 10):
        int_list2.add(i)
    int_list2.remove(1)
    int_list2.remove(3)
    int_list2.remove(2)
    int_list2.remove(0)
    dieifnot(str(int_list2), "9 8 7 6 5 4")

    # Test 4
    for i in range(11, 13):
        int_list2.append(i)
    dieifnot(str(int_list2), "9 8 7 6 5 4 11 12")

    # Test 5
    for i in range(21, 23):
        int_list2.insert(0, i)
    dieifnot(str(int_list2), "22 21 9 8 7 6 5 4 11 12")
    dieifnot(int_list2.get_size(), 10)

    # Test 6
    int_list = d_linked_list()
    dieifnot(int_list.get_size(), 0)

    # Test 7
    for i in range(0, 1000):
        int_list.append(i)
    correctOrder = True
    dieifnot(int_list.get_size(), 1000)

    # Test 8
    for i in range(0, 200):
        if int_list.pop() != 999 - i:
            correctOrder = False
    dieifnot(correctOrder)
    print("TEST:  Correct order test passed.")
    dieifnot(int_list.search_larger(200), 201)

    # Test 9
    int_list.insert(7, 801)
    dieifnot(int_list.search_larger(800), 7)
    dieifnot(int_list.get_item(-1), 799)
    dieifnot(int_list.get_item(-4), 796)

    if is_pass is True:
        s = "==========="
        print("%s Congratulations! You have finished exercise 1! %s" % (s, s))

    int_list.pop(0)
    dieifnot(int_list.get_item(0), 1)
    int_list.append(None)
    dieifnot(int_list.pop(), None, 1)
    int_list.pop(5000)


###############################################################################
# Main


if __name__ == '__main__':
    test()
