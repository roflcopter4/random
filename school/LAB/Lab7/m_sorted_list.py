#!/usr/bin/python3

from d_linked_list import d_linked_list

###############################################################################
# Classes


class m_sorted_list(d_linked_list):
    """This class doesn't even know why it exists, let alone what it does."""

    def __init__(self, m_sorted):
        super().__init__()
        self.isSorted = m_sorted

    def add(self, item):
        if (not self.isSorted) or (self.get_size() == 0):
            super().append(item)
        else:
            pos = super().search_larger(item)
            if pos == 0:
                super().add(item)
            elif pos == -1:
                super().append(item)
            else:
                super().insert(pos, item)

    def pop(self):
        return super().pop(0 - int(self.isSorted))

    def search(self, item):
        index = super().index(item)
        if index == -1:
            if self.isSorted:
                if item >= self.get_item(-1):
                    index = -1
                else:
                    index = super().search_larger(item)
            retval = False
        else:
            retval = True

        return retval, index

    def change_sorted(self):
        if self.isSorted:
            self.isSorted = False
        else:
            raise LazyProgrammer("I don't know how to do that.")


class LazyProgrammer(Exception):
    """Laaaazy"""

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
    sor_list = m_sorted_list(True)
    dieifnot(sor_list.get_size(), 0)

    sor_list.add(4)
    sor_list.add(3)
    sor_list.add(8)
    sor_list.add(7)
    sor_list.add(1)

    dieifnot(str(sor_list), "1 3 4 7 8")
    dieifnot(sor_list.get_size(), 5)
    dieifnot(sor_list.pop(), 8)
    dieifnot(sor_list.pop(), 7)
    dieifnot(str(sor_list), "1 3 4")

    a = sor_list.search(2)
    dieifnot(a[0] is False and a[1] == 1)

    a = sor_list.search(3)
    dieifnot(a[0] is True and a[1] == 1)

    a = sor_list.search(7)
    dieifnot(a[0] is False and a[1] == -1)
    dieifnot(sor_list.get_size(), 3)
    dieifnot(sor_list.get_item(2), 4)

    sor_list.change_sorted()
    sor_list.add(1)

    dieifnot(str(sor_list), "1 3 4 1")
    dieifnot(sor_list.get_size(), 4)
    dieifnot(sor_list.pop(), 1)
    dieifnot(sor_list.pop(), 3)

    sor_list.add(7)
    sor_list.add(6)
    dieifnot(str(sor_list), "4 1 7 6")

    a = sor_list.search(2)
    dieifnot(a[0] is False and a[1] == -1)

    a = sor_list.search(7)
    dieifnot(a[0] is True and a[1] == 2)

    a = sor_list.search(8)
    dieifnot(a[0] is False and a[1] == -1)

    dieifnot(sor_list.get_size(), 4)
    dieifnot(sor_list.get_item(2), 7)

    sor_list2 = m_sorted_list(False)
    dieifnot(sor_list2.get_size(), 0)

    sor_list2.add(4)
    sor_list2.add(3)
    sor_list2.add(8)
    sor_list2.add(7)
    sor_list2.add(1)

    dieifnot(str(sor_list2), "4 3 8 7 1")
    dieifnot(sor_list2.get_size(), 5)
    dieifnot(sor_list2.pop(), 4)
    dieifnot(sor_list2.pop(), 3)
    dieifnot(str(sor_list2), "8 7 1")

    a = sor_list2.search(2)
    dieifnot(a[0] is False and a[1] == -1)

    a = sor_list2.search(7)
    dieifnot(a[0] is True and a[1] == 1)

    dieifnot(sor_list2.get_size(), 3)
    dieifnot(sor_list2.get_item(2), 1)

    try:
        sor_list2.change_sorted()
    except Exception as e:
        print(e)
        is_pass = True
    else:
        dieifnot(False)

    sor_list2.add(3)
    sor_list2.add(2)

    dieifnot(str(sor_list2), "8 7 1 3 2")
    dieifnot(sor_list2.get_size(), 5)
    dieifnot(sor_list2.pop(), 8)
    dieifnot(sor_list2.pop(), 7)
    dieifnot(str(sor_list2), "1 3 2")

    if is_pass:
        s = "==========="
        print("%s Congratulations! You have finished exercise 2! %s" % (s, s))


###############################################################################
# Main


if __name__ == '__main__':
    test()
