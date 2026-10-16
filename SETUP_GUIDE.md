# ESP32 CAM Setup Guide - Complete Installation Instructions

## Installation Status: ✅ Complete

All necessary packages have been installed successfully on your system.

### Installed Components

1. **PlatformIO 6.2.0** - Installed via pip
   - Location: Python site-packages
   - Command: `platformio`
   - Status: Ready to use

2. **Python 3.13.15** - Already available
   - Status: Ready

3. **Node.js v22.14.0** - Already available
   - Status: Ready

4. **Git 2.55.0** - Already available
   - Status: Ready

## Quick Start

### 1. Configure Your WiFi

Edit `src/main.cpp` and update these lines:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 2. Build the Project

```bash
cd "c:\Users\nandhakumar\OneDrive\Documents\New folder"
platformio run -e esp32-cam
```

Or use npm script:
```bash
npm run build
```

### 3. Upload to ESP32-CAM

```bash
platformio run -e esp32-cam -t upload
```

Or use npm script:
```bash
npm run upload
```

**Before uploading:**
- Connect ESP32-CAM via USB adapter
- Ensure COM port is detected in Device Manager
- If not detected, install CH340 drivers

### 4. Monitor Serial Output

```bash
platformio device monitor -e esp32-cam --baud 115200
```

Or use npm script:
```bash
npm run monitor
```

You will see:
- Camera initialization status
- WiFi connection status
- IP address assigned by your router
- Any error messages

### 5. Access the Camera Feed

1. Find the IP address in the serial monitor output
2. Open your browser
3. Navigate to: `http://<IP_ADDRESS>`
4. You should see the live camera feed

## Available npm Scripts

```bash
npm run build      # Compile the project
npm run upload     # Upload to ESP32
npm run monitor    # View serial output
npm run clean      # Clean build artifacts
npm run install-libs # Install additional libraries
```

## Hardware Setup

### ESP32-CAM Pinout
- **USB Connection**: Via USB-to-Serial adapter (CH340 or FTDI)
- **Power**: 5V, 500mA minimum
- **Reset Pin**: GPIO0 (for flashing mode)
- **IO0 to GND**: For firmware upload

### USB Adapter Connection
```
USB Adapter → ESP32-CAM
VCC → 5V
GND → GND
TX → RX (GPIO3)
RX → TX (GPIO1)
```

## Troubleshooting

### "platformio: command not found"
- Windows PowerShell doesn't recognize pip installations by default
- Use: `python -m platformio` instead
- Or add Python\Scripts to PATH environment variable

### Port Not Found
```bash
# List available COM ports
platformio device list
```

### Failed to Upload
1. Check Device Manager for COM port
2. Ensure GPIO0 is grounded during upload
3. Try different baud rates (115200, 921600)
4. Use external power supply (5V/2A)

### Camera Initialization Failed
- Check all camera pin connections
- Verify power supply
- Try different frame sizes
- Check PSRAM availability

### WiFi Connection Fails
- Verify SSID and password are correct
- Ensure network is 2.4GHz (5GHz not supported)
- Check signal strength at ESP32 location
- Try connecting to a different network

## Project Structure

```
esp32-cam-project/
├── src/
│   └── main.cpp              # Main firmware code
├── lib/                       # Custom libraries (auto-created)
├── include/                   # Header files (auto-created)
├── platformio.ini            # PlatformIO configuration
├── package.json              # NPM scripts
├── config.h                  # Configuration constants
├── README.md                 # Project readme
└── SETUP_GUIDE.md           # This file
```

## Next Steps

### 1. Customize Web Interface
Edit the HTML in `main.cpp`:
```cpp
server.on("/", HTTP_GET, []() {
  server.send(200, "text/html", R"(
    <!-- Your custom HTML here -->
  )");
});
```

### 2. Add Image Processing
- Motion detection
- Face detection
- Object recognition
- Image filters

### 3. Enhance Functionality
- Add SD card logging
- Implement OTA updates
- Add WebSocket streaming
- Email alert notifications

### 4. Data Logging
- Log images to SD card
- Store sensor data
- Create time-lapse sequences

## Useful Resources

- [ESP32 Camera Documentation](https://github.com/espressif/esp32-camera)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)
- [ESP32 Official Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)

## Support Commands

View device list:
```bash
platformio device list
```

Show environment info:
```bash
platformio env list
```

Remote update boards:
```bash
platformio update
```

Check library updates:
```bash
platformio lib update
```

## License

MIT License - Feel free to use and modify
