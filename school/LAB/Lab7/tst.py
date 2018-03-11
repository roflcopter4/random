#!/usr/bin/env python3

from d_linked_list import d_linked_list
from timeit import repeat
import random


def get_items(Qlen):
    return [int(10000*random.random()) for _ in range(Qlen)]


def fill_queue(queue, items):
    for i in items:
        queue.enqueue(i)


Qlen = 100000000
queue = d_linked_list()
items = get_items(Qlen)

for i in items:
    queue.append(i)

result = 0
result = repeat("for i in range(Qlen-1): queue.pop(0)",
                globals=globals(), number=1, repeat=1)

total = sum(result)
print("Result was: ", total, "sec.")
