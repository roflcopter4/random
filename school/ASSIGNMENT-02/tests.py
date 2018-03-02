#!/usr/bin/env python3

import sys
from copy import deepcopy
import game


def testCollapseRow():
    print("Running tests for collapseRow()")
    grid = game.Grid()

    tests = [['a', [2, 0,  0,  0], ([ 2,  0, 0,  0], False)],
             ['b', [2, 0,  2,  0], ([ 4,  0, 0,  0], True )],
             ['c', [2, 2,  2,  0], ([ 4,  2, 0,  0], True )],
             ['d', [2, 0,  2,  2], ([ 4,  2, 0,  0], True )],
             ['e', [8, 8, 16,  8], ([16, 16, 8,  0], True )],
             ['f', [2, 0,  2,  4], ([ 4,  4, 0,  0], True )],
             ['g', [2, 8,  4,  4], ([ 2,  8, 8,  0], True )],
             ['e', [0, 0,  0,  0], ([ 0,  0, 0,  0], True )],
             ['f', [2, 0,  0,  2], ([ 4,  0, 0,  0], True )],
             ['g', [2, 4,  8, 16], ([ 2,  4, 8, 16], False)],
             ['h', [8, 8,  8,  8], ([16, 16, 0,  0], True )]]

    ID = 0
    INIT = 1
    SOL = 2
    for test in tests:
        ret = grid.collapseRow(deepcopy(test[INIT]))
        if ret == test[SOL]:
            print("Test (%s) passed." % test[ID])
        else:
            print("Test (%s) failed." % test[ID])


def testCollapseLeft():
    print("Running test for collapseLeft()")
    grid = game.Grid()
    grid._grid = [[0, 0, 0, 0],
                  [0, 0, 0, 4],
                  [2, 0, 2, 16],
                  [2, 4, 4, 2]]

    # orig = deepcopy(grid._grid)
    sol = [[0, 0, 0, 0],
           [4, 0, 0, 0],
           [4, 16, 0, 0],
           [2, 8, 2, 0]]

    ret = grid.collapseLeft()
    test = grid._grid

    # print(orig)
    # print(test)
    # print(sol)

    if sol == test and ret:
        print("Left collapse test passed.")
    else:
        print("Test case failed.")


def testCollapseRight():
    print("Running test for collapseRight()")
    grid = game.Grid()
    grid._grid = [[16, 0, 16, 0],
                  [0, 2, 4, 4],
                  [2, 2, 2, 0],
                  [2, 4, 4, 2]]

    # orig = deepcopy(grid._grid)
    sol = [[0, 0, 0, 32],
           [0, 0, 2, 8],
           [0, 0, 2, 4],
           [0, 2, 8, 2]]

    ret = grid.collapseRight()
    test = grid._grid

    # print(orig)
    # print(test)
    # print(sol)

    if sol == test and ret:
        print("Right collapse test passed.")
    else:
        print("Test case failed.")


def testCollapseDown():
    print("Running test for collapseDown()")
    grid = game.Grid()
    grid._grid = [[2, 8, 2, 4],
                  [8, 4, 8, 2],
                  [2, 4, 2, 4],
                  [4, 2, 4, 2]]

    sol = [[2, 0, 2, 4],
           [8, 8, 8, 2],
           [2, 8, 2, 4],
           [4, 2, 4, 2]]

    grid.collapseDown()
    test = grid._grid

    if sol == test:
        print("Down collapse test passed.")
    else:
        print("Test case failed.")


def testCollapsible():
    print("Running test for collapsible()")
    grid = game.Grid()

    grid.emptiesSet = [0, 1, 2, 3, 4, 5, 6, 8]
    grid._grid = [[0, 0, 0, 0],
                  [0, 0, 0, 4],
                  [2, 0, 2, 16],
                  [2, 4, 4, 2]]

    if grid.collapsible():
        print('Test (a) passed.')
    else:
        print('Test (a) failed.')

    grid.emptiesSet = []
    grid._grid = [[2, 8, 2, 4],
                  [4, 4, 8, 2],
                  [2, 8, 2, 4],
                  [4, 2, 4, 2]]

    if grid.collapsible():
        print('Test (b) passed.')
    else:
        print('Test (b) failed.')

    grid.emptiesSet = []
    grid._grid = [[2, 8, 2, 4],
                  [8, 4, 8, 2],
                  [2, 4, 2, 4],
                  [4, 2, 4, 2]]

    if grid.collapsible():
        print('Test (c) passed.')
    else:
        print('Test (c) failed.')

    grid.emptiesSet = []
    grid._grid = [[2, 4, 2, 4],
                  [4, 2, 4, 2],
                  [2, 4, 2, 4],
                  [4, 2, 4, 2]]

    if not grid.collapsible():
        print('Test (d) passed.')
    else:
        print('Test (d) failed.')


def testEmpties():
    print("Running test for updateEmptiesSet()")
    grid = game.Grid()
    grid._grid = [[0, 0, 0, 0],
                  [0, 0, 0, 4],
                  [2, 0, 2, 16],
                  [2, 4, 4, 0]]
    grid.updateEmptiesSet()
    truth = [0, 1, 2, 3, 4, 5, 6, 9, 15]
    if grid.emptiesSet == truth:
        print('Empties set test passed.')
    else:
        print('Empties set test failed.')


# This is much more convenient than uncommenting lines.

def pick_test(test):
    ret = True
    try:
        if test in ('collapsible', '1'):
            testCollapsible()
        elif test in ('collapserow', 'crow', 'row', '2'):
            testCollapseRow()
        elif test in ('collapseleft', 'left', 'l', '3'):
            testCollapseLeft()
        elif test in ('collapsedown', 'down', 'd', '4'):
            testCollapseDown()
        elif test in ('empties', 'e', '5'):
            testEmpties()
        elif test in ('collapseright', 'right', 'r', '6'):
            testCollapseRight()
        elif test in ('all', 'a'):
            for t in range(1, 7):
                pick_test(str(t))
                print()
            sys.exit()  # Force exit if 'all' tests were chosen
        else:
            print("Invalid argument: '%s'" % test)
            ret = False

    except AttributeError:
        print('Not implemented yet.')
        ret = False

    return ret


should_exit = False
first = True
for arg in sys.argv[1:]:
    should_exit = pick_test(arg) or should_exit
    print()

while not should_exit:
    if first:
        first = False
        print("""\
1. Test collapsible() function.
2. Test collapseRow() function.
3. Test collapseLeft() function.
4. Test collapseDown() function.
5. Test empties() function.
6. Test collapseRight() function.
Type enter, EOF, or an interrupt to exit.""")

    try:
        buf = input('\nYour choice: ')
    except (KeyboardInterrupt, EOFError):
        sys.exit()
    if buf in ('', '\n', '\r', '\r\n'):
        sys.exit()
    print()
    pick_test(buf)
