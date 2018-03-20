#!/usr/bin/env python3

from datetime import date

# =============================================================================
# Functions

def parse_lines(f):
    data = []
    line = f.readline()
    while line != '':
        line = line.rstrip('\n')
        data.append(line.split(' '))
        line = f.readline()
    return data

# =============================================================================
# Main

filename = "earthquake.txt"
try:
    f = open(filename, 'r')
except IOError:
    print("ERROR: File not found, exiting.")

data_raw = parse_lines(f)
data_filt = []
region_list = []
for line in data_raw:
    qRegion    = line[-1]
    qMagnitude = line[0]
    qDate      = line[1]
    data_filt.append([qRegion, qMagnitude, qDate])
    region_list.append(qRegion)

region_list = sorted(set(region_list))

data_final = []
for region in region_list:
    qList = [region]
    for quake in data_filt:
        if quake[0] == region:
            qList.append(quake[1:])

    data_final.append(qList)

for point in data_final:
    region = point[0]
    print(region.ljust(10) + ':', point[1:])
