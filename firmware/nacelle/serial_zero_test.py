import serial
import time

ser = serial.Serial('/dev/serial0', 115200)  # Default UART on Pi is /dev/serial0 (or /dev/ttyAMA0)
#GPIO 14 (TX) and GPIO 15 (RX)
# Wait for the connection to establish
time.sleep(2)

# Send a serial message
ser.write(b'Hello, Raspberry Pi!\n')

# Close the serial connection when done
ser.close()
