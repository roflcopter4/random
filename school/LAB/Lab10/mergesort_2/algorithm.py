#!/usr/bin/env python3


def quick_sort(data):
    # Sort myself using a quick sort.
    quick_sort_helper(data, 0, len(data) - 1)


def quick_sort_helper(data, first, last):
    if first < last:
        pivot = partition(data, first, last)      # partition around a pivot
        quick_sort_helper(data, first, pivot - 1)  # sort 1st half
        quick_sort_helper(data, pivot + 1, last)   # sort 2nd half


def partition(data, first, last):
    # choosing the pivot as the first element in the list
    pivot_val = data[first]

    # left_mark indicates the end of the first partition (+1)
    left_mark = first + 1

    # right_mark indicates the beginning of the second partition
    right_mark = last

    done = False

    while not done:
        while (left_mark <= right_mark) and (data[left_mark] <= pivot_val):
            left_mark = left_mark + 1    # shifting the pointer to the right
        while (right_mark >= left_mark) and (data[right_mark] >= pivot_val):
            right_mark = right_mark - 1  # shifting the pointer to the left

        # the partitioning is done
        if right_mark < left_mark:
            done = True

        # elements blocking the partitioning need to be swaped around pivot
        else:
            temp = data[left_mark]
            data[left_mark] = data[right_mark]
            data[right_mark] = temp

    temp = data[first]  # putting pivot in place
    data[first] = data[right_mark]
    data[right_mark] = temp
    return right_mark


###############################################################################


mylist = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
quick_sort(mylist)
print(mylist)
