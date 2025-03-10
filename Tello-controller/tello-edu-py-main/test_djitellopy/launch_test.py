from djitellopy import Tello
import time
import serial

ser = serial.Serial('/dev/serial0', 115200)  # Default UART on Pi is /dev/serial0 (or /dev/ttyAMA0)
tello = Tello(host="192.168.93.187")
tello.connect()
time.sleep(1)

answer = ""
while(answer is not "load"):
    answer = input("Select next order")

loaded = False
# Connect to the drone
ser.write("load".encode())

while(loaded is False):
   msg =  ser.readall()
   print(msg) if msg is not None else None  
   loaded = True if msg == "loaded" else False

print("takeoff")
# Start flight

#tello.initiate_throw_takeoff()
#time.sleep(2)

#ser.write(b"0\n")  # Send as raw bytes

#GPIO 14 (TX) and GPIO 15 (RX)
# Wait for the connection to establish

# Close the serial connection when done
ser.close()


# Disconnect
tello.end()
