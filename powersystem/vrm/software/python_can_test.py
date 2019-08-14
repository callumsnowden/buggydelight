import can
import websockets
import sys
import json
import asyncio

bus = can.interface.Bus(bustype='kvaser', channel=0, bitrate=500000, sjw=2, tseg1=13, tseg2=2)

async def ConvertVoltages():
	for msg in bus:
		input_voltage = int(msg.data[4])
		aux_voltage = int(msg.data[2])
		main_voltage = int(msg.data[0])

		voltages = {
		"input": input_voltage,
		"main": main_voltage,
		"aux": aux_voltage
		}

		return json.dumps(voltages)

async def producer_handler(websocket, path):
	while True:
		message = await ConvertVoltages()
		await websocket.send(message)

ws_server = websockets.serve(producer_handler, "localhost", 5678)
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