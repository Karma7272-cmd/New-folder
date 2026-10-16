# ESP32 CAM Embedded Coding Project & Vercel Web Dashboard

This project provides a complete setup for ESP32 CAM development with web server capabilities and a modern front-end web dashboard tailored for **Vercel** deployment.

## Features

- **Embedded ESP32-CAM Firmware**: Real-time camera streaming, HTTP WebServer, and FOMO object detection API.
- **Web Dashboard (Vercel Ready)**: Modern UI for viewing live camera feed, real-time bounding box AI overlay, FPS counter, and detection analytics.
- **Dual Connection Mode**: Supports connecting directly to a physical ESP32-CAM on your local network/IP or operating in an interactive Demo/Standalone mode.
- **Vercel Deployable**: Includes `vercel.json` and static asset bundle ready for 1-click Vercel deployment.

---

## 🌐 Deploying the Web Dashboard to Vercel

### Method 1: Deploy via Vercel CLI

1. Install the Vercel CLI (if not already installed):
   ```bash
   npm i -g vercel
   ```
2. Run the deployment command from the project root:
   ```bash
   vercel
   ```
3. Follow the prompts to complete the deployment.

### Method 2: Deploy via GitHub & Vercel Dashboard

1. Push this repository to GitHub/GitLab/Bitbucket.
2. Go to [Vercel Dashboard](https://vercel.com/new).
3. Import this repository.
4. Vercel will automatically detect the settings from `vercel.json` and `package.json`.
5. Click **Deploy**.

---

## 🔌 Connecting Vercel Web Interface to ESP32-CAM

1. Flash your ESP32-CAM using PlatformIO / Arduino IDE with the source code in `src/main.cpp`.
2. Connect the ESP32-CAM to your local Wi-Fi.
3. Note the IP address printed in the Serial Monitor (e.g. `192.168.1.105`).
4. Open your deployed Vercel web application URL.
5. In the top-right header, enter your ESP32-CAM IP address into the **Device Address** box and click **Connect**.
6. Switch from **Demo Stream** to **Live Device** mode to view live camera streams and detection metrics from your embedded hardware!

> **Note on CORS / Mixed Content**: When accessing a local HTTP ESP32 IP address from an HTTPS Vercel domain, ensure your browser allows mixed content, or use a secure tunnel (such as Ngrok or LocalTunnel) pointing to your local ESP32 IP.

---

## 🛠️ Software Installation & PlatformIO

### Prerequisites

- Python 3.6+
- Node.js 16+
- Git

### Build Commands

- **Build Vercel Web Assets**:
  ```bash
  npm run build
  ```
- **Build ESP32 Firmware**:
  ```bash
  npm run build:esp32
  ```
- **Upload to ESP32**:
  ```bash
  npm run upload
  ```
- **Monitor Serial Output**:
  ```bash
  npm run monitor
  ```

---

## 📁 Project Structure

```
.
├── public/                # Web Dashboard static files (HTML/CSS/JS)
│   ├── index.html
│   ├── style.css
│   └── app.js
├── src/
│   └── main.cpp           # ESP32 CAM C++ Sketch
├── vercel.json            # Vercel deployment configuration
├── platformio.ini         # PlatformIO configuration
├── package.json           # Node.js scripts
└── README.md              # Project documentation
```

## License

MIT
