#!/usr/bin/env python3
import subprocess
import threading
import random
import os

# We may need to guard this?
child = subprocess.Popen('./cunt',
                         stdout=subprocess.PIPE,
                         stdin=subprocess.PIPE)


def print_child():
    """Continuously print what the process outputs..."""
    for line in child.stdout:
        print(line)
    # child.stdout.flush()
    # print(child.stdout.readline())
    # print("Got a line done")


child_reader = threading.Thread(target=print_child)
child_reader.start()

# chars = ''.join([j if j.isprintable() else '' for j in [chr(i) for i in range(127)]])
for _ in range(10):
    for _ in range(5):
        child.stdin.write("%8d ".encode('ascii') % random.randint(0, 500000))
    child.stdin.write('\n'.encode('ascii'))
    # child.stdin.flush()
    # child.stdout.flush()

    # fd = os.dup(child.stdin.fileno())
    # os.dup2(child.stdin.fileno(), 1)
    # child.stdin.close()
    child_reader.run()
    # child.wait()
    # child_reader.join()
    # child.stdin = os.fopen(fd)

    # child.wait()
    # child_reader.join()
    # child_reader.run()

# Send EOF. This kills the cat.

# I don't think order matters here?
child.wait()
child_reader.join()
