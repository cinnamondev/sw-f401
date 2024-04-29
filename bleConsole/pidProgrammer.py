# pidProgrammer
# input syntax
# key kp ki kd
# cinnamondev

import asyncio
from bleak import *
import aioconsole
from ctypes import c_float

device_name = "GROUP_30"
device_characteristic = "0000ffe1-0000-1000-8000-00805f9b34fb"

# input from bluetooth response (read)
def console_in(characteristic: BleakGATTCharacteristic, data: bytearray):
    print(data.decode("utf-8"), end="")

async def main():
    d = await BleakScanner.find_device_by_name("GROUP_30") # Find device by name
    async with BleakClient(d) as client: # GATT Client for device
        c = client.services.get_characteristic(device_characteristic) # UART Emulation Characteristic
        if c is not None:
            # print data when sent by hm10
            await client.start_notify(c, console_in)
            await client.write_gatt_char(c, str.encode("B")) # "ping" command (shows functionality)
            while True: # const
                user_input = await aioconsole.ainput("")
                user_input.split(" ")
                key = bytes(user_input[0]) # process user input and represent as
                kp = bytes(c_float(float(user_input[1])).value) # bytes for transfer
                ki = bytes(c_float(float(user_input[2])).value)
                kd = bytes(c_float(float(user_input[3])).value)
                output = bytearray([str.encode("T"), key, kp, ki, kd])
                await client.write_gatt_char(c, output)

asyncio.run(main())
