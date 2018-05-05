#!/usr/bin/env python3
import os
from asyncproc import Process
myProc = Process("myprogram.app")

while True:
    # check to see if process has ended
    poll = myProc.wait(os.WNOHANG)
    if poll is None:
        break
    # print any new output
    out = myProc.read()
    if out != "":
        print(out)
