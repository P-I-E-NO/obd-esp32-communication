import serial
import serial.tools.list_ports
import requests
import json
import os
import sys
from typing import Optional

class Bridge():

    
    def __init__(self) -> None:
        self.serial()

    
    def serial(self) -> None:  
        ports = serial.tools.list_ports.comports()
        port_name = None

        self.ser = None
        for port in ports:
            if "USB" in port.device and "USB to UART" in port.description:
                print("Successfully connected to microcontroller")
                port_name = port.device

        try:
            if port_name is not None:
                self.ser = serial.Serial(port_name, 115200, timeout=0)
        except:
            self.ser = None


    def loop(self) -> None:
        self.inbuffer = bytearray()
        while True:
            if self.ser is not None:
                if self.ser.in_waiting > 0:
                    lastchar = self.ser.read(1)
                    if lastchar == b'\xff':
                        while True:
                            if lastchar == b'\xfe':
                                tank = self.get_tank()
                                if tank != -1:
                                    self.post_data(tank)
                                self.inbuffer = bytearray()
                                break

                            self.inbuffer.extend(lastchar)
                            lastchar = self.ser.read(1)


    def get_tank(self) -> Optional[float]:
        string = self.inbuffer.decode("utf-8", "ignore").replace('\n', '').replace('\r', '').split('>')

        tank_level_resp = string[string.__len__()-1].split()

        try:
            tank_level = tank_level_resp[2:][3]
            return (int(tank_level, 16) * 100 / 255)
        except IndexError:
            return -1
        
    
    def post_data(self, tank:float) -> None:
        token = input("Insert your token here:\n")
        
        if token == '':
            token = os.environ["TMP_TOKEN_PIENO"]
        
        headers = {"Authorization":f"Bearer {token}", 
                   "Content-Type": "application/json"}
        data = {"value": round(tank)}
        print(f'Sending tank data to server: {round(tank)}%')
        response = requests.post('https://api.pieno.cloud/meter/', data=json.dumps(data), headers=headers)
        print(response.status_code)
        print(response.json())



if __name__ == '__main__':
    br = Bridge()
    br.loop()