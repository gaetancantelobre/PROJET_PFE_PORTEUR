from djitellopy import Tello
import time
import serial

ser = serial.Serial('/dev/serial0', 115200)  # Default UART on Pi is /dev/serial0 (or /dev/ttyAMA0)
tello = Tello(host="192.168.93.187")
tello.connect()
time.sleep(1)

answer = ""
while(answer != "load"):
    answer = input("Select next order")
    print(answer)

loaded = False
# Connect to the drone
ser.write("load\n".encode())

while(loaded is False):
   msg = ser.readline().decode().strip()
   print(msg) if msg is not None else None  
   loaded = msg.strip() == "loaded"


answer = ""
while(answer != "launch"):
    answer = input("When ready for launch : \"launch\" ")

print("takeoff")
ser.write("launch\n".encode())

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
