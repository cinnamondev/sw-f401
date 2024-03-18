# BLE Console
# Console for BLE characteristic of device! cinnamondev

import asyncio
try:
    from bleak import *
    import aioconsole
except:
    print("please install bleak")

def console_in(characteristic: BleakGATTCharacteristic, data: bytearray):
    print(data.decode("utf-8"))

device_name = "GROUP_30"
device_characteristic = "0000ffe1-0000-1000-8000-00805f9b34fb"

async def main():
    stdin,stdout = await aioconsole.get_standard_streams()
    d = await BleakScanner.find_device_by_name("GROUP_30")
    print(d)
    async with BleakClient(d) as client:
        c = client.services.get_characteristic(device_characteristic)
        if c is not None:
            await client.start_notify(c, console_in)
            while True:
                user_input = await aioconsole.ainput("")
                await client.write_gatt_char(c, str.encode(user_input))

asyncio.run(main())
