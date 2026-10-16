#!/usr/bin/env python3
import serial
import time

print("=" * 70)
print("ESP32 CAM - Reading Serial Output")
print("=" * 70)
print()

try:
    ser = serial.Serial('COM4', 115200, timeout=1)
    time.sleep(1)
    
    print("Reading output (30 seconds)...\n")
    
    output = ""
    start = time.time()
    found_ip = False
    
    while time.time() - start < 30:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print(line)
                    output += line + "\n"
                    
                    if "IP address" in line:
                        found_ip = True
                        print("\n" + "=" * 70)
                        print("FOUND IP ADDRESS!")
                        print("=" * 70)
                        break
        except:
            pass
        
        time.sleep(0.05)
    
    ser.close()
    
    if not found_ip:
        print("\n" + "=" * 70)
        print("Output captured:")
        print("=" * 70)
        print(output)
        
except Exception as e:
    print(f"Error: {e}")
