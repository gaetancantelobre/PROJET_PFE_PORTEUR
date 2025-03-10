import nmap
import socket

def scan_network():
    nm = nmap.PortScanner()
    
    # Get the local network IP range
    hostname = socket.gethostbyname(socket.gethostname())
    network_prefix = ".".join(hostname.split(".")[:-1]) + ".0/24"

    print(f"Scanning network: {network_prefix}...")
    
    # Scan for devices with port 8889 open (Tello's default port)
    nm.scan(hosts=network_prefix, arguments='-p 8889 --open')

    tello_ips = [host for host in nm.all_hosts() if '8889/tcp' in nm[host]['tcp']]

    if tello_ips:
        print(f"✅ Tello EDU Drone Found! IP: {tello_ips[0]}")
        return tello_ips[0]
    else:
        print("❌ No Tello EDU Drone found on the network.")
        return None

# Run the scanner
tello_ip = scan_network()
