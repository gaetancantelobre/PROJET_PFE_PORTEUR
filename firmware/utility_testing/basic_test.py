from djitellopy import Tello
import time

# Connect to the drone
tello = Tello(host="192.168.93.187")

tello.connect()
print(f"Battery: {tello.get_battery()}%")  # Check battery level

# Start flight
tello.takeoff()

# Move the drone
tello.flip_forward()  # Move forward 50 cm

tello.land()  # Land the drone

# Disconnect
tello.end()
