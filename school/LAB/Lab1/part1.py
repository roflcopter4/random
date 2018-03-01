#!/usr/bin/env python3

# =============================================================================
# Functions

def parse_lines(f):
    strList = []
    line = f.readline()
    while line != '':
        line = line.rstrip('\n')
        strList.append(line.split(' '))
        line = f.readline()
    return strList

# =============================================================================
# Main

filename = "rainfall.txt"
try:
    f = open(filename, 'r')
except IOError:
    print("ERROR: File not found, exiting.")

data = parse_lines(f)
data = list(map(lambda x: [x[0], float(x[1])], data))
data = sorted(data, key=lambda x: x[1])

lowerBounds = list(range(51, 92, 10))
upperBounds = list(range(60, 101, 10))

for Iter in range(0, 5):
    isEmpty = True
    lbound = lowerBounds[Iter]
    ubound = upperBounds[Iter]
    for pair in data:
        city = pair[0].upper()
        rainfall = pair[1]
        if rainfall > lbound and rainfall <= ubound:
            if isEmpty:
                isEmpty = False
                print('[' + str(lbound) + ' - ' + str(ubound) + ']')
            city = city + (17 - len(city)) * ' '
            rainfall = "%5.1f" %rainfall
            print('\t' + city, rainfall)
