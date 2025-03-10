#!/usr/bin/python

import sys
import os

import asyncio
import tello_edu_protocol as tello
from tello_edu_protocol.commands import (
    takeoff,
    cw,
    ccw,
    forward,
    land,
    # initiate_throw_takeoff,
    emergency,
    get_battery
)

async def main() -> None:
    async with tello.conn(timeout=20) as drone:
        print(f"Battery: {await drone.command(get_battery)}%")
        #1 - try the throw take off approach and see if the drone reacts as wished
        #2 - if (1) doesn't work, try the get_height/get_acceleration_z approach 
        # await drone.command(takeoff)
        await drone.command(takeoff)
        # await drone.command(cw, 180)
        await drone.command(forward, 60)
        # await drone.command(ccw, 180)
        # await drone.command(forward, 40)
        await drone.command(land)


if __name__ == '__main__':
    asyncio.run(main())
