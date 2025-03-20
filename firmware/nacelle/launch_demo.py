from djitellopy import Tello
import time
import serial
import json

with open("demo_settings.json", "r") as file:
    config = json.load(file)

# Assign values to variables
DRONE_IP = config["drone_ip"]
ADD_FLIP = config["add_flip"]

ser = serial.Serial('/dev/serial0', 115200)  # Default UART on Pi is /dev/serial0 (or /dev/ttyAMA0)
tello = Tello(host=DRONE_IP)
tello.connect()
time.sleep(1)

answer = ""
while(answer != "load"):
    answer = input("Select next order ? \n")

print("You can load the drone now.\n")
loaded = False
# Connect to the drone
ser.write("load\n".encode())

while(loaded is False):
   msg = ser.readline().decode().strip()
   print(msg) if msg is not None else None  
   loaded = msg.strip() == "loaded"

print("Drone is loaded.")

answer = ""
while(answer != "launch"):
    answer = input("When ready for launch : \"launch\" ")

print("takeoff")
tello.initiate_throw_takeoff()
time.sleep(1)
ser.write("launch\n".encode())
time.sleep(3)

# Start flight
tello.move_forward(30)
if(ADD_FLIP):
    tello.flip_forward()
    tello.flip_back()


ser.close()

tello.land()
# Disconnect
tello.end()
