import serial
import serial.tools.list_ports
import requests

#print("available ports: ")
ports = serial.tools.list_ports.comports()
port_name = None

for port in ports:
    if "USB" in port.device and "USB to UART" in port.description:
        port_name = port.device

try:
    if port_name is not None:
        ser = serial.Serial(port_name, 115200, timeout=0)
except:
    ser = None

inbuffer = bytearray()
i = 0
while(True):
    if not ser is None:
        if ser.in_waiting > 0:
            lastchar = ser.read(1)

            inbuffer.extend(lastchar)
            i += 1
    
    if i == 200:
        break


print(inbuffer)