#!/usr/bin/env python2

from subprocess import Popen, PIPE

proc = Popen(["python2", "./echoprocess.py"],
             stdin=PIPE,
             stdout=PIPE)

proc.stdin.write("hello\n")
# proc.stdin.flush()
print proc.stdout.readline()
