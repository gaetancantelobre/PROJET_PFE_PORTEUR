import serial
import time

ser = serial.Serial('/dev/serial0', 9600)  # Default UART on Pi is /dev/serial0 (or /dev/ttyAMA0)
#GPIO 14 (TX) and GPIO 15 (RX)
# Wait for the connection to establish
time.sleep(2)
sent_string = input("What message would you like to send to pi zero 2 W ??")
# Send a serial message
ser.write(sent_string)

# Close the serial connection when done
ser.close()
