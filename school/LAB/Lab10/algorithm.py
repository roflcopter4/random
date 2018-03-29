#!/usr/bin/env python3


def merge(left, right):
    result = []
    lc, rc = 0, 0
    while lc < len(left) and rc < len(right):
        if left[lc] <= right[rc]:
            result.append(left[lc])
            lc += 1
        else:
            result.append(right[rc])
            rc += 1
    result += left[lc:]
    result += right[rc:]
    return result


def merge_sort(data):
    if len(data) < 2:
        return data
    middle = len(data) // 2
    left = merge_sort(data[:middle])
    right = merge_sort(data[middle:])
    return merge(left, right)
