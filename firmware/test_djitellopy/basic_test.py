from djitellopy import Tello
import time

# Connect to the drone
tello = Tello(host="192.168.93.187")

tello.connect()
print(f"Battery: {tello.get_battery()}%")  # Check battery level

# Start flight
tello.initiate_throw_takeoff()

# Move the drone
tello.move_forward(50)  # Move forward 50 cm

tello.land()  # Land the drone

# Disconnect
tello.end()
