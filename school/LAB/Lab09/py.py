#!/usr/bin/env python3


def selectionSort(alist):
    for place in range((len(alist)-1), 0, -1):
        pos_max = 0
        for location in range(1, place + 1):
            if alist[location] > alist[pos_max]:
                pos_max = location

        temp = alist[place]
        alist[place] = alist[pos_max]
        alist[pos_max] = temp


alist = [54, 26, 93, 17, 77, 31, 44, 55, 20]
selectionSort(alist)
print(alist)
