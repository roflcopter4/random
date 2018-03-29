import game

def testCollapseRow():
    print("Running tests for collapseRow()")
    grid = game.Grid()
    
    a = [2, 0, 0, 0]
    b = [2, 0, 2, 0]
    c = [2, 2, 2, 0]
    d = [2, 0, 2, 2]
    e = [8, 8, 16, 8]
    f = [2, 0, 2, 4]
    g = [2, 8, 4, 4]

    a_sol = ([2, 0, 0, 0], False)
    b_sol = ([4, 0, 0, 0], True)
    c_sol = ([4, 2, 0, 0], True)
    d_sol = ([4, 2, 0, 0], True)
    e_sol = ([16, 16, 8, 0], True)
    f_sol = ([4, 4, 0, 0], True)
    g_sol = ([2, 8, 8, 0], True)

    if grid.collapseRow(a) == a_sol:
        print("Test (a) passed.")
    else:
        print("Test (a) failed")

    if grid.collapseRow(b) == b_sol:
        print("Test (b) passed.")
    else:
        print("Test (b) failed")

    if grid.collapseRow(c) == c_sol:
        print("Test (c) passed.")
    else:
        print("Test (c) failed")

    if grid.collapseRow(d) == d_sol:
        print("Test (d) passed.")
    else:
        print("Test (d) failed")

    if grid.collapseRow(e) == e_sol:
        print("Test (e) passed.")
    else:
        print("Test (e) failed")

    if grid.collapseRow(f) == f_sol:
        print("Test (f) passed.")
    else:
        print("Test (f) failed")

    if grid.collapseRow(g) == g_sol:
        print("Test (g) passed.")
    else:
        print("Test (g) failed")


def testCollapseLeft():
    print("Running test for collapseLeft()")
    grid = game.Grid()
    grid._grid = [[0, 0, 0, 0],
                   [0, 0, 0, 4],
                   [2, 0, 2, 16],
                   [2, 4, 4, 2]]
                   
    sol = [[0, 0, 0, 0],
           [4, 0, 0, 0],
           [4, 16, 0, 0],
           [2, 8, 2, 0]]
    
    grid.collapseLeft()
    test = grid._grid
    
    if sol == test:
        print("Left collapse test passed.")
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



# Uncomment the tests for the function you want to check

testCollapseRow()
testCollapseLeft()
testCollapseDown()
testCollapsible()
testEmpties()
