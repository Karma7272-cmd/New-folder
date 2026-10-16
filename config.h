#ifndef CONFIG_H
#define CONFIG_H

// Camera Configuration
#define CAMERA_MODEL_ESP32_CAM

// WiFi Configuration
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// Web Server Configuration
#define SERVER_PORT 80
#define SERIAL_BAUDRATE 115200

// Camera Settings
#define DEFAULT_JPEG_QUALITY 10  // 0-63 (lower = better quality)
#define DEFAULT_FRAMESIZE FRAMESIZE_VGA  // VGA, SVGA, UXGA, etc.
#define DEFAULT_BRIGHTNESS 0  // -2 to 2
#define DEFAULT_CONTRAST 0    // -2 to 2
#define DEFAULT_SATURATION 0  // -2 to 2

// Frame buffer
#define FRAME_BUFFER_COUNT 2

// Debug
#define DEBUG_SERIAL true
#define DEBUG_LEVEL 2  // 0=none, 1=error, 2=warning, 3=info, 4=debug

// PSRAM
#define USE_PSRAM true

// Flash
#define FLASH_GPIO_PIN 4  // Optional flash LED

#endif // CONFIG_H
