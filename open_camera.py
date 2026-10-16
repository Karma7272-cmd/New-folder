#!/usr/bin/env python3
import webbrowser
import subprocess
import time

print("=" * 60)
print("ESP32 CAM Browser Opener")
print("=" * 60)

# List of common IP addresses to try
ips_to_try = [
    "192.168.1.100",
    "192.168.0.100",
    "192.168.1.101",
    "192.168.0.101",
    "10.0.0.100",
    "172.16.0.100",
]

print("\nTrying common IP addresses...\n")

found = False
for ip in ips_to_try:
    url = f"http://{ip}/"
    print(f"Trying {ip}...", end=" ")
    
    try:
        # Try to ping
        result = subprocess.run(
            f"ping -n 1 -w 1000 {ip}",
            shell=True,
            capture_output=True,
            timeout=5
        )
        
        if result.returncode == 0:
            print("✓ RESPONDING!")
            print(f"\nOpening browser to: {url}")
            webbrowser.open(url)
            found = True
            break
        else:
            print("No response")
    except:
        print("Error")

if not found:
    print("\n" + "=" * 60)
    print("ESP32 not found on common IPs")
    print("=" * 60)
    print("\nManual steps:")
    print("1. Open your WiFi router settings")
    print("2. Look for 'Connected Devices' or 'DHCP Clients'")
    print("3. Find the ESP32 device (MAC: 3c:8a:1f:d5:27:60)")
    print("4. Copy its IP address")
    print("5. Open browser and go to: http://<IP_ADDRESS>/")
    print("\nOr try these manually:")
    for ip in ips_to_try:
        print(f"  - http://{ip}/")
