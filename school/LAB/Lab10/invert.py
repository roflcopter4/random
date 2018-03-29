#!/usr/bin/env python3

total = bast = whore = 0
nig = 0


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

        # print((left, right, result))

    global total, bast, whore
    total += len(left) - lc
    bast += lc + rc

    print(left, right)
    if left[-1] > right[0]:
        whore += 1

    result += left[lc:]
    result += right[rc:]

    global nig
    nig += len(result)
    print(nig)
    # print("%s - %s - %s" % (str(left), str(right), str(result)))

    return result


def merge_sort(data):
    if len(data) < 2:
        return data
    middle = len(data) // 2
    left = merge_sort(data[:middle])
    right = merge_sort(data[middle:])
    return merge(left, right)


# my_list = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
# my_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
my_list = [1, 2, 10, 4, 5, 8, 7]
my_list = merge_sort(my_list)
print(my_list)
print(total, bast, whore)
# print(total, )
