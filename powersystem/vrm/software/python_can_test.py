import can
import websockets
import sys
import json
import asyncio

bus = can.interface.Bus(bustype='kvaser', channel=0, bitrate=500000, sjw=2, tseg1=13, tseg2=2)

on_msg = can.Message(data=[1], arbitration_id=0x320, dlc=1, is_extended_id=False)
off_msg = can.Message(data=[0], arbitration_id=0x320, dlc=1, is_extended_id=False)

async def ConvertVoltages():
	for msg in bus:
		input_voltage = int(msg.data[4])
		aux_voltage = int(msg.data[2])
		main_voltage = int(msg.data[0])
		main_current = int(msg.data[1]/10)
		aux_current = int(msg.data[3]/10)

		values = {
		"input_voltage": input_voltage,
		"main_voltage": main_voltage,
		"main_current": main_current,
		"aux_voltage": aux_voltage,
		"aux_current": aux_current
		}

		return json.dumps(values)

async def SendCommand(message):
	if(command == "on"):
		bus.send(on_msg)
	if(command == "off"):
		bus.send(off_msg)

async def consumer_handler(websocket, path):
	async for message in websocket:
		print(message)
		await SendCommand(message)

async def producer_handler(websocket, path):
	while True:
		message = await ConvertVoltages()
		await websocket.send(message)

async def handler(websocket, path):
    consumer_task = asyncio.ensure_future(
        consumer_handler(websocket, path))
    producer_task = asyncio.ensure_future(
        producer_handler(websocket, path))
    done, pending = await asyncio.wait(
        [consumer_task, producer_task],
        return_when=asyncio.FIRST_COMPLETED,
    )
    for task in pending:
        task.cancel()

ws_server = websockets.serve(handler, "localhost", 5678)
asyncio.get_event_loop().run_until_complete(ws_server)
asyncio.get_event_loop().run_forever()

'''
for msg in bus:
	input_voltage = int(msg.data[4])
	aux_voltage = int(msg.data[2])
	main_voltage = int(msg.data[0])
	sys.stdout.write("\rInput: " + str(input_voltage) + "V, Aux: " + str(aux_voltage) + "V, Main: " + str(main_voltage) + "V")
	sys.stdout.flush()
'''