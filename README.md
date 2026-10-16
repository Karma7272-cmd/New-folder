# ESP32 CAM Embedded Coding Project

This project provides a complete setup for ESP32 CAM development with web server capabilities.

## Features

- Camera initialization and configuration
- WiFi connectivity
- Web server streaming
- JPEG image capture
- Real-time monitoring via serial console

## Hardware Requirements

- ESP32-CAM board
- USB to Serial adapter (TTL)
- Micro USB cable for power
- WiFi network

## Software Installation

### 1. Prerequisites

Ensure you have installed:
- Python 3.6+
- Node.js 12+
- Git

### 2. Install PlatformIO

PlatformIO is already installed globally. To verify:

```bash
platformio --version
```

### 3. Install Project Dependencies

```bash
npm install
```

## Configuration

### WiFi Setup

Edit `src/main.cpp` and update the WiFi credentials:

```cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```

### Board Selection

The project is configured for ESP32-CAM. Verify in `platformio.ini`:

```ini
board = esp32-camera
```

## Building and Uploading

### Build the project:

```bash
npm run build
```

### Upload to ESP32:

```bash
npm run upload
```

Make sure:
- ESP32 is connected via USB
- Device appears in Device Manager (COMx port)
- Drivers are installed (CH340 or FTDI depending on adapter)

### Monitor Serial Output:

```bash
npm run monitor
```

## Web Access

1. After upload completes, the ESP32 will connect to your WiFi
2. Open the serial monitor to see the IP address
3. Access the web server: `http://<IP_ADDRESS>`
4. The page displays live camera feed
5. Click "Refresh" to capture new frames

## Troubleshooting

### Failed to Connect to COM Port

- Check Device Manager for the correct COM port
- Verify USB cable is properly connected
- Ensure drivers are installed (CH340 drivers: https://sparks.gogo.co.nz/ch340.html)

### Camera Initialization Failed

- Verify all camera pin connections
- Check power supply (5V, at least 500mA)
- Ensure correct board selection

### WiFi Connection Issues

- Verify SSID and password are correct
- Check if network is 2.4GHz (ESP32 doesn't support 5GHz)
- Ensure router has WiFi enabled

### Image Quality Issues

- Adjust `jpeg_quality` in `main.cpp` (0-63, lower = better quality)
- Adjust `frame_size` for resolution (FRAMESIZE_VGA, FRAMESIZE_SVGA, etc.)

## Port Information

- Web Server: Port 80
- Serial Monitor: 115200 baud

## Project Structure

```
.
├── src/
│   └── main.cpp           # Main sketch
├── platformio.ini         # PlatformIO configuration
├── package.json           # Node.js scripts
└── README.md             # This file
```

## References

- [ESP32 Camera GitHub](https://github.com/espressif/esp32-camera)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Official Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)

## Library Documentation

### Key Libraries Used

1. **esp32-camera**: Official Espressif camera library
2. **ArduinoJson**: JSON processing
3. **WebSockets**: Real-time communication
4. **ESP8266-OLED**: Display support (optional)

## Next Steps

1. Customize the web interface in the HTML response
2. Add image processing functionality
3. Implement motion detection
4. Add data logging to SD card
5. Implement OTA updates

## License

MIT
