# import subprocess
#
# proc = subprocess.Popen(['./neotags', '780'], universal_newlines=True,
#                         stdin=subprocess.PIPE, stdout=subprocess.PIPE)
#
#
# # print(data)
#
# proc.stdin.write(data)
# proc.stdin.write("MAKE\n")
# # out = proc.stdout.readline()
# # print(out)
# proc.stdin.write("STOP\n")
# out = proc.stdout.read(1)
# # out, err = proc.communicate()
# print(out)


# import sys
# from subprocess import PIPE, Popen
# from threading import Thread
#
# try:
#     from Queue import Queue, Empty
# except ImportError:
#     from queue import Queue, Empty  # python 3.x
#
# ON_POSIX = 'posix' in sys.builtin_module_names
#
#
# def enqueue_output(out, queue):
#     for line in iter(out.readline, b''):
#         queue.put(line)
#     out.close()
#
#
# with open('Makefile', 'r') as fp:
#     data = ''.join([i for i in fp])
#
# p = Popen(['./neotags', '780'], stdout=PIPE, stdin=PIPE, universal_newlines=True, bufsize=1, close_fds=ON_POSIX)
# q = Queue()
# t = Thread(target=enqueue_output, args=(p.stdout, q))
# t.daemon = True  # thread dies with the program
# t.start()
#
# p.stdin.write(data)
# p.stdin.write("MAKE\n")
#
# # ... do other things here
#
# # read line without blocking
# try:
#     line = q.get_nowait()  # or q.get(timeout=.1)
# except Empty:
#     print('no output yet')
# else:
#     print(line)
#
# p.stdin.write("STOP\n")

# import fcntl
# import os
# import sys
#
# # make stdin a non-blocking file
# fd = sys.stdin.fileno()
# fl = fcntl.fcntl(fd, fcntl.F_GETFL)
# fcntl.fcntl(fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)
#
# # user input handling thread
# while mainThreadIsRunning:
#     try:
#         Input = sys.stdin.readline()
#     except:
#         continue
#     handleInput(Input)

# import os
# import asyncproc
#
#
# def dostuff(proc, thing):
#     myProc.write("MAKE\n")
#     while True:
#         myProc.wait(os.WNOHANG)
#         # myProc.write("STOP\n")
#         out = myProc.read()
#         if out != "":
#             print(out)
#             return
#
#
# with open('Makefile', 'r') as fp:
#     data = ''.join([i for i in fp])
#
# # check to see if process has ended
# myProc = asyncproc.Process(["./neotags", '780'], universal_newlines=True)
# myProc.write(data)
#
# dostuff(myProc, "MAKE\n")
# dostuff(myProc, "blargth\n")
# dostuff(myProc, "INT\n")
# dostuff(myProc, "CC\n")
# dostuff(myProc, "hello\n")
# dostuff(myProc, "STOP\n")

# from twisted.internet import protocol, reactor
#
#
# class MyProcessProtocol(protocol.ProcessProtocol):
#
#     def outReceived(self, data):
#         print(data)
#
#
# proc = MyProcessProtocol()
# reactor.spawnProcess(proc, './myprogram', ['./myprogram', 'arg1', 'arg2', 'arg3'])
# reactor.run()


# import tornado_subprocess
# import tornado.ioloop
#
#
# def print_res(status, stdout, stderr):
#     print(status, stdout, stderr)
#     if status == 0:
#         print("OK:")
#         print(stdout)
#     else:
#         print("ERROR:")
#         print(stderr)
#
#
# t = tornado_subprocess.Subprocess(
#     print_res, timeout=30, args=["cat", "/etc/passwd"])
# t.start()
# tornado.ioloop.IOLoop.instance().start()


# from subprocess import Popen, PIPE
# from threading import Thread
#
#
# def process_output(myprocess):  # output-consuming thread
#     nextline = None
#     buf = ''
#     while True:
#         # --- extract line using read(1)
#         out = myprocess.stdout.read(1)
#         if out == '' and myprocess.poll() is not None:
#             break
#         if out != '':
#             buf += out
#             if out == '\n':
#                 nextline = buf
#                 buf = ''
#         if not nextline:
#             continue
#         line = nextline
#         nextline = None
#
#         # --- do whatever you want with line here
#         print('Line is:', line)
#     myprocess.stdout.close()
#
#
# myprocess = Popen(['./neotags', '780'], stdout=PIPE, universal_newlines=True)
# p1 = Thread(target=process_output, args=(myprocess))  # output-consuming thread
# p1.daemon = True
# p1.start()
#
# # --- do whatever here and then kill process and thread if needed
# if myprocess.poll() is None:  # kill process; will automatically stop thread
#     myprocess.kill()
#     myprocess.wait()
# if p1 and p1.is_alive():  # wait for thread to finish
#     p1.join()


# import fcntl
# import os
# from subprocess import Popen, PIPE
# 
# 
# def non_block_read(output):
#     fd = output.fileno()
#     fl = fcntl.fcntl(fd, fcntl.F_GETFL)
#     fcntl.fcntl(fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)
#     try:
#         return output.read()
#     except Exception:
#         return ""
# 
# 
# # Use example
# sb = Popen("echo test && sleep 1000", shell=True, stdout=PIPE)
# sb.kill()
# 
# # sb.stdout.read() # <-- This will block
# non_block_read(sb.stdout)
# 'test\n'

import subprocess
from subprocess import PIPE
import os
import pty

# cmd = ["./neotags", '780']
cmd = ['cat']

# master, slave = pty.openpty()
# process = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE, stdout=slave)
# stdin_handle = process.stdin
# stdout_handle = os.fdopen(master)
# stdout = os.fdopen(master)
# 
# stdin_handle.write(b"hello")
# out = stdout_handle.readline()
# print(out)

# #!/usr/bin/python
# from subprocess import Popen, PIPE, STDOUT
# import pty
# import os
# 
# cmd = 'socat -d -d PTY: PTY:'

master, slave = pty.openpty()

p = subprocess.Popen(cmd, shell=True, stdin=PIPE, stdout=slave, stderr=slave, close_fds=True)
# stdout = os.fdopen(master)
# p.stdin.write(b'hello\n')
# print(stdout.readline())
# print(stdout.readline())
print(p.communicate(input=b'hi'))
