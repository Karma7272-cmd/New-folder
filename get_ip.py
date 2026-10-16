#!/usr/bin/env python3
import serial
import time
import webbrowser

print("=" * 60)
print("ESP32 CAM IP Address Finder")
print("=" * 60)

try:
    # Connect to ESP32
    ser = serial.Serial('COM4', 115200, timeout=1)
    print("Connected to COM4")
    print("Waiting for ESP32 to boot and connect to WiFi...")
    print("-" * 60)
    
    ip_address = None
    start_time = time.time()
    max_wait = 30  # seconds
    
    while time.time() - start_time < max_wait:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print(line)
                    
                    # Look for IP address
                    if "IP address:" in line:
                        # Extract IP
                        parts = line.split(":")
                        if len(parts) >= 2:
                            ip_address = parts[-1].strip()
                            print("\n" + "=" * 60)
                            print(f"FOUND IP: {ip_address}")
                            print("=" * 60)
                            break
        except Exception as e:
            pass
        
        time.sleep(0.1)
    
    ser.close()
    
    if ip_address:
        url = f"http://{ip_address}/"
        print(f"\nOpening browser to: {url}")
        print("Waiting 3 seconds before opening...")
        time.sleep(3)
        webbrowser.open(url)
        print("Browser should open now!")
    else:
        print("\nNo IP address found after 30 seconds")
        print("Troubleshooting:")
        print("1. Check if ESP32 is powered on")
        print("2. Verify WiFi SSID and password are correct")
        print("3. Check your WiFi router settings")
        print("4. Try opening http://192.168.1.100 manually")

except Exception as e:
    print(f"Error: {e}")
    print("Make sure ESP32 is connected to COM4")
