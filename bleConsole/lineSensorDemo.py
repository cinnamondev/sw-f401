# Line sensor output loop

import asyncio
from bleak import *
import aioconsole

device_name = "GROUP_30"
device_characteristic = "0000ffe1-0000-1000-8000-00805f9b34fb"

# input from bluetooth response (read)
def console_in(characteristic: BleakGATTCharacteristic, data: bytearray):
    print(data.decode("utf-8"))

async def main():
    d = await BleakScanner.find_device_by_name("GROUP_30") # Find device by name
    async with BleakClient(d) as client: # GATT Client for device
        c = client.services.get_characteristic(device_characteristic) # UART Emulation Characteristic
        if c is not None:
            # print data when sent by hm10
            await client.start_notify(c, console_in)
            client.write_gatt_char(c, str.encode("B")) # "ping" command (shows functionality)
            while True:
                await client.write_gatt_char(c, str.encode("L")) # get line sensor data
                await asyncio.sleep(1)

asyncio.run(main())
