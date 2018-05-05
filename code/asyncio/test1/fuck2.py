#!/usr/bin/env python3
import asyncio
import locale
import sys
import random
from asyncio.subprocess import PIPE
from contextlib import closing


async def readline_and_kill(*args):
    process = await asyncio.create_subprocess_exec(*args, stdin=PIPE, stdout=PIPE)

    for _ in range(100):
        process.stdin.write("%d\n".encode('ascii') % random.randint(0, 50000))
        async for line in process.stdout:
            print("got line:", line.decode(locale.getpreferredencoding(False)))
            break

    process.kill()
    return await process.wait()


if sys.platform == "win32":
    loop = asyncio.ProactorEventLoop()
    asyncio.set_event_loop(loop)
else:
    loop = asyncio.get_event_loop()

with closing(loop):
    loop.run_until_complete(readline_and_kill("./cunt"))
