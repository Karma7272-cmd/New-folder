# ESP32 CAM Quick Reference Card

## Essential Commands

### Build
```bash
npm run build
platformio run -e esp32-cam
```

### Upload
```bash
npm run upload
platformio run -e esp32-cam -t upload
```

### Monitor
```bash
npm run monitor
platformio device monitor -e esp32-cam --baud 115200
```

### Clean
```bash
npm run clean
platformio run -e esp32-cam -t clean
```

### List Devices
```bash
platformio device list
```

---

## WiFi Configuration

**File:** `src/main.cpp` (lines 29-30)

```cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```

---

## Camera Settings

**File:** `src/main.cpp` (lines 72-73)

```cpp
config.frame_size = FRAMESIZE_VGA;     // Resolution
config.jpeg_quality = 10;               // Quality (0-63, lower=better)
```

### Frame Size Options
- FRAMESIZE_QQVGA (160×120)
- FRAMESIZE_QVGA (320×240)
- FRAMESIZE_VGA (640×480) ← Default
- FRAMESIZE_SVGA (800×600)
- FRAMESIZE_UXGA (1600×1200)

### Quality Scale
- 0-10: High quality (slower, larger file)
- 10-30: Medium quality (balanced)
- 30-63: Low quality (faster, smaller file)

---

## Hardware Connections

### USB Adapter to ESP32
```
VCC  → 5V
GND  → GND
TX   → RX (GPIO3)
RX   → TX (GPIO1)
```

### Upload Mode (Flashing)
- Hold GPIO0 to GND
- Press Reset
- Release after 2 seconds

---

## Troubleshooting

### Command Not Found
```bash
# Use Python directly
python -m platformio
```

### No COM Port Detected
1. Check Device Manager
2. Install USB drivers (DRIVER_SETUP.md)
3. Try different USB cable
4. Try different USB port

### Upload Failed
1. Ensure GPIO0 is grounded
2. Check power supply (5V, 500mA+)
3. Try lower baud rate in platformio.ini
4. Verify correct COM port

### WiFi Won't Connect
1. Check SSID and password
2. Ensure 2.4GHz network (5GHz not supported)
3. Check signal strength
4. Verify network is open or WPA2-Personal

### Black/Corrupted Image
1. Check camera ribbon cable
2. Verify camera power
3. Try higher JPEG quality
4. Reduce frame size

---

## File Locations

| File | Purpose |
|------|---------|
| `src/main.cpp` | Basic example |
| `src/advanced_example.cpp` | Advanced features |
| `platformio.ini` | Build configuration |
| `config.h` | Constants |
| `package.json` | npm scripts |

---

## Web Server Access

After upload and WiFi connection:

1. Check serial monitor for IP: `192.168.x.x`
2. Open browser: `http://192.168.1.100`
3. Default endpoints:
   - `/` - Web interface
   - `/capture` - JPEG image
   - `/api/stats` - JSON statistics

---

## Serial Monitor Output

Expected sequence:
```
ESP32 CAM Web Server
Starting...
Camera initialized successfully
Connecting to WiFi: YOUR_SSID
....
WiFi connected
IP address: 192.168.1.100
Camera server started
```

---

## Performance Tips

### Faster Upload Speed
```ini
upload_speed = 921600
```

### Lower Latency
```cpp
config.jpeg_quality = 25;     // Faster capture
config.frame_size = FRAMESIZE_QVGA;  // Smaller size
```

### Better Image Quality
```cpp
config.jpeg_quality = 5;      // Higher quality
config.frame_size = FRAMESIZE_UXGA;  // Larger size
```

---

## Power Requirements

- **5V Supply:** Minimum 500mA
- **Recommended:** 1A-2A for stability
- **USB Hub:** May not provide enough power

---

## Status Indicators

| Indicator | Meaning |
|-----------|---------|
| Serial output | Firmware running |
| WiFi connected | IP visible in serial |
| Web server ready | Can access /capture |
| Image displayed | Camera working |

---

## Advanced Features (in `advanced_example.cpp`)

- Real-time FPS counter
- Request statistics
- JSON API responses
- Signal strength monitoring

---

## Useful Links

- Docs: https://docs.platformio.org/
- ESP32: https://github.com/espressif/arduino-esp32
- Camera: https://github.com/espressif/esp32-camera

---

## One-Minute Checklist

- [ ] Drivers installed
- [ ] USB adapter connected
- [ ] platformio recognized
- [ ] WiFi credentials updated
- [ ] `npm run build` succeeds
- [ ] `npm run upload` completes
- [ ] `npm run monitor` shows output
- [ ] Web server accessible

---

**Next:** See SETUP_GUIDE.md for detailed walkthrough
