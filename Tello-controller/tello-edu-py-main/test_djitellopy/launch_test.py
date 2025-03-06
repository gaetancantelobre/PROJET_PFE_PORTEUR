from djitellopy import Tello
import time
import serial

ser = serial.Serial('/dev/serial0', 115200)  # Default UART on Pi is /dev/serial0 (or /dev/ttyAMA0)
# Connect to the drone
tello = Tello(host="192.168.93.187")
tello.connect()
time.sleep(2)

# Start flight
tello.initiate_throw_takeoff()
time.sleep(1)

ser.write(b"90\n")  # Send as raw bytes

#GPIO 14 (TX) and GPIO 15 (RX)
# Wait for the connection to establish

# Close the serial connection when done
ser.close()


# Move the drone


tello.land()  # Land the drone

# Disconnect
tello.end()
