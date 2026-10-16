# USB Adapter Driver Setup

## Overview

ESP32-CAM requires a USB-to-Serial adapter for programming and monitoring. The most common adapters use CH340 or FTDI chips.

## Identify Your Adapter

### In Device Manager (Windows):
1. Open Device Manager
2. Look under "Ports (COM & LPT)"
3. Unknown device → CH340 (needs driver)
4. COM port visible → Working (may need confirmation)

## CH340 Driver Installation

### Method 1: Automatic (Recommended)

Windows may auto-detect and install drivers. If not:

1. Go to [CH340 Driver Download](https://sparks.gogo.co.nz/ch340.html)
2. Download for your Windows version (32-bit or 64-bit)
3. Extract the ZIP file
4. Run the installer (.exe)
5. Restart your computer
6. Plug in USB adapter

### Method 2: Manual Installation

1. Download CH340 driver
2. Open Device Manager
3. Right-click Unknown Device → Update driver
4. Browse my computer → Let me pick from list
5. Select the CH340 driver from extracted folder
6. Follow prompts to install

### Method 3: Windows Update

1. Plug in USB adapter
2. Open Device Manager
3. Right-click Unknown Device → Update driver
4. Search automatically for updated driver software
5. Windows may download and install automatically

## FTDI Driver Installation

If your adapter uses FTDI chip:

1. Go to [FTDI Driver Page](https://ftdichip.com/drivers/)
2. Download "CDM 21228 Drivers"
3. Extract and run installer
4. Follow installation prompts
5. Restart computer

## Verify Installation

### Check COM Port Assignment:

```bash
# Using PowerShell
Get-WmiObject Win32_SerialPort | Select-Object Name, Description, DeviceID
```

Or in Device Manager:
- Ports (COM & LPT)
- Should show "USB Serial Port (COMx)"

### Test Connection:

```bash
platformio device list
```

Should show your COM port and ESP32 device

## Troubleshooting

### Still Unknown Device

1. Download driver directly from chip manufacturer
2. Manually select driver in Device Manager
3. Disable driver signature verification (not recommended)
4. Try different USB cable
5. Try different USB port

### COM Port Disappears

- Try different USB cable
- Try different computer
- Check power supply to ESP32
- Update adapter firmware (FTDI adapters sometimes need this)

### Upload Fails Despite Visible COM Port

1. Ensure GPIO0 is held to GND during upload
2. Try baud rate: 115200
3. Add delay before upload: `sleep 2000` in platformio.ini
4. Check USB cable quality

### Multiple COM Ports

- Disconnect other USB devices
- Use Device Manager to identify correct port
- Specify port in platformio.ini:

```ini
upload_port = COM3
monitor_port = COM3
```

## platformio.ini Configuration

```ini
[env:esp32-cam]
platform = espressif32
board = esp32-camera
framework = arduino

; Specify COM port
upload_port = COM3
monitor_port = COM3

; Baud rates
upload_speed = 921600
monitor_speed = 115200

; USB Settings
upload_speed = 115200  ; For older adapters
```

## Alternative: Arduino IDE

If PlatformIO doesn't work:

1. Download [Arduino IDE](https://www.arduino.cc/en/software)
2. Add ESP32 board support
3. Install CH340 drivers separately
4. Select board and COM port
5. Upload sketch

## Recommended Adapters

### CH340 Adapters
- Generic "USB-to-Serial CP2102" $2-5
- Cheap, widely available, works well

### FTDI Adapters
- Better quality, more expensive ($15-30)
- Official support, more reliable

## Next Steps

After driver installation:

1. Connect ESP32-CAM via USB adapter
2. Verify COM port in Device Manager
3. Run `platformio device list`
4. Execute: `npm run build`
5. Execute: `npm run upload`

## Additional Resources

- [ESP32 Hardware Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [CH340 Documentation](https://sparks.gogo.co.nz/ch340.html)
- [PlatformIO Troubleshooting](https://docs.platformio.org/en/latest/faq/troubleshooting.html)
