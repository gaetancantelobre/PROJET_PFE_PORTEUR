#!/usr/bin/python

import sys
import os

import asyncio
import tello_edu_protocol as tello
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '/Tello-controller/tello-edu-py-main/tello_edu_protocol/'))
from tello_edu_protocol.commands import (
    takeoff,
    cw,
    ccw,
    forward,
    land,
    get_battery
)

async def main() -> None:
    async with tello.conn(timeout=20) as drone:
        print(f"Battery: {await drone.command(get_battery)}%")
        await drone.command(takeoff)
        # await drone.command(cw, 180)
        # await drone.command(forward, 40)
        # await drone.command(ccw, 180)
        # await drone.command(forward, 40)
        await drone.command(land)


if __name__ == '__main__':
    asyncio.run(main())
