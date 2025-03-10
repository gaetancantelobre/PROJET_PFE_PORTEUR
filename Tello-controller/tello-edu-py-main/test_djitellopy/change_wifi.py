from djitellopy import Tello

# Replace with your Wi-Fi SSID and Password
WIFI_SSID = "Projet_porteur"
WIFI_PASSWORD = "aspic2025"

# Connect to the drone
tello = Tello() 
tello.connect()

# Send the STA mode command
tello.connect_to_wifi(WIFI_SSID, WIFI_PASSWORD)

print("Tello is switching to Wi-Fi network:", WIFI_SSID)
