#!/usr/bin/python3

from d_linked_list import d_linked_list


###############################################################################
# Classes


class m_sorted_list:

    def __init__(self, m_sorted):
        pass

    def add(self, item):
        pass

    def pop(self):
        pass

    def search(self, item):
        pass

    def change_sorted(self):
        pass

    def get_size(self):
        pass

    def get_item(self, pos):
        pass

    def __str__(self):
        pass


###############################################################################
# Functions


def dieifnot(condition):
    fail_msg = "You have failed the test and shamed yourself and your country."
    assert condition, fail_msg


def test():
    sor_list = m_sorted_list(True)
    dieifnot(sor_list.get_size() == 0)

    sor_list.add(4)
    sor_list.add(3)
    sor_list.add(8)
    sor_list.add(7)
    sor_list.add(1)

    dieifnot(str(sor_list) == "1 3 4 7 8")
    dieifnot(sor_list.get_size() == 5)
    dieifnot(sor_list.pop() == 8)
    dieifnot(sor_list.pop() == 7)
    dieifnot(str(sor_list) == "1 3 4")

    a = sor_list.search(2)
    dieifnot(a[0] is False and a[1] == 1)

    a = sor_list.search(3)
    dieifnot(a[0] is True and a[1] == 1)

    a = sor_list.search(7)
    dieifnot(a[0] is False and a[1] == -1)
    dieifnot(sor_list.get_size() == 3)
    dieifnot(sor_list.get_item(2) == 4)

    sor_list.change_sorted()
    sor_list.add(1)

    dieifnot(str(sor_list) == "1 3 4 1")
    dieifnot(sor_list.get_size() == 4)
    dieifnot(sor_list.pop() == 1)
    dieifnot(sor_list.pop() == 3)

    sor_list.add(7)
    sor_list.add(6)
    dieifnot(str(sor_list) == "4 1 7 6")

    a = sor_list.search(2)
    dieifnot(a[0] is False and a[1] == -1)

    a = sor_list.search(7)
    dieifnot(a[0] is True and a[1] == 2)

    a = sor_list.search(8)
    dieifnot(a[0] is False and a[1] == -1)

    dieifnot(sor_list.get_size() == 4)
    dieifnot(sor_list.get_item(2) == 7)

    sor_list2 = m_sorted_list(False)
    dieifnot(sor_list2.get_size() == 0)

    sor_list2.add(4)
    sor_list2.add(3)
    sor_list2.add(8)
    sor_list2.add(7)
    sor_list2.add(1)

    dieifnot(str(sor_list2) == "4 3 8 7 1")
    dieifnot(sor_list2.get_size() == 5)
    dieifnot(sor_list2.pop() == 4)
    dieifnot(sor_list2.pop() == 3)
    dieifnot(str(sor_list2) == "8 7 1")

    a = sor_list2.search(2)
    dieifnot(a[0] is False and a[1] == -1)

    a = sor_list2.search(7)
    dieifnot(a[0] is True and a[1] == 1)

    dieifnot(sor_list2.get_size() == 3)
    dieifnot(sor_list2.get_item(2) == 1)

    try:
        sor_list2.change_sorted()
    except Exception as e:
        is_pass = True
    else:
        dieifnot(False)

    sor_list2.add(3)
    sor_list2.add(2)

    dieifnot(str(sor_list2) == "8 7 1 3 2")
    dieifnot(sor_list2.get_size() == 5)
    dieifnot(sor_list2.pop() == 8)
    dieifnot(sor_list2.pop() == 7)
    dieifnot(str(sor_list2) == "1 3 2")

    if is_pass:
        s = "==========="
        print("%s Congratulations! You have finished exercise 2! %s" % (s, s))


###############################################################################
# Main


if __name__ == '__main__':
    test()
