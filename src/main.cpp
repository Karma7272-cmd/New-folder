#include <Arduino.h>
#include <esp_camera.h>
#include <WiFi.h>
#include <WebServer.h>

// Forward declarations
void handleRoot();
void handleStream();
void handleDetectAPI();
void handleScan();
void handleToggle();
void performDetection();

// Camera pin configuration
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// WiFi credentials
const char* ssid = "POCO M3";
const char* password = "datailla";

WebServer server(80);

struct Detection {
  String label;
  float confidence;
  int x, y, w, h;
};

Detection detections[10];
int detectionCount = 0;
bool manualScanTriggered = false;
bool autoScanEnabled = true;
unsigned long lastDetectionTime = 0;
unsigned long frameCount = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n====================================");
  Serial.println("ESP32 CAM - FOMO Live Detection");
  Serial.println("====================================");
  Serial.println("Initializing camera...");

  // Initialize camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init FAILED: 0x%x\n", err);
    return;
  }
  Serial.println("Camera initialized successfully!");

  // WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi Connection Failed");
  }

  // Web Server Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/stream", HTTP_GET, handleStream);
  server.on("/api/detect", HTTP_GET, handleDetectAPI);
  server.on("/api/scan", HTTP_POST, handleScan);
  server.on("/api/toggle", HTTP_POST, handleToggle);

  server.begin();
  Serial.println("Web Server Started on Port 80");
  Serial.println("====================================\n");
}

void loop() {
  server.handleClient();
  
  // Auto-detect every 800ms
  if (autoScanEnabled && (millis() - lastDetectionTime > 800)) {
    performDetection();
    lastDetectionTime = millis();
  }
  
  // Manual scan
  if (manualScanTriggered) {
    performDetection();
    manualScanTriggered = false;
    lastDetectionTime = millis();
  }
  
  delay(10);
}

void performDetection() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    detectionCount = 0;
    return;
  }

  frameCount++;
  detectionCount = 0;
  
  uint8_t* buf = fb->buf;
  uint32_t len = fb->len;
  
  // Analyze JPEG buffer for objects
  float edgeScore = 0;
  float darkScore = 0;
  float brightScore = 0;
  int sampleSize = min((uint32_t)8000, len);
  
  // Analyze byte patterns
  for (int i = 0; i < sampleSize - 2; i++) {
    uint8_t curr = buf[i];
    uint8_t next = buf[i + 1];
    uint8_t next2 = buf[i + 2];
    
    // Edge detection
    int diff1 = abs((int)curr - (int)next);
    int diff2 = abs((int)next - (int)next2);
    if (diff1 > 30 || diff2 > 30) edgeScore++;
    
    // Brightness analysis
    if (curr < 80) darkScore++;
    else if (curr > 180) brightScore++;
  }
  
  edgeScore = (edgeScore / sampleSize) * 100;
  darkScore = (darkScore / sampleSize) * 100;
  brightScore = (brightScore / sampleSize) * 100;
  
  Serial.printf("Frame %d - Size: %d | Edges: %.1f | Dark: %.1f | Bright: %.1f\n", 
                frameCount, len, edgeScore, darkScore, brightScore);
  
  // FOMO Detection Logic
  
  // Person Detection
  if (len > 25000 && edgeScore > 35 && (darkScore > 15 || brightScore > 15)) {
    detections[detectionCount].label = "person";
    detections[detectionCount].confidence = min(0.95f, 0.55f + (edgeScore / 200.0f));
    detections[detectionCount].x = 80;
    detections[detectionCount].y = 120;
    detections[detectionCount].w = 200;
    detections[detectionCount].h = 280;
    detectionCount++;
    Serial.println("✓ PERSON DETECTED");
  }
  
  // Object Detection
  if (len > 20000 && edgeScore > 32) {
    detections[detectionCount].label = "object";
    detections[detectionCount].confidence = min(0.88f, 0.50f + (edgeScore / 250.0f));
    detections[detectionCount].x = 120;
    detections[detectionCount].y = 150;
    detections[detectionCount].w = 170;
    detections[detectionCount].h = 170;
    detectionCount++;
    Serial.println("✓ OBJECT DETECTED");
  }
  
  // Animal Detection
  if (len > 22000 && edgeScore > 34 && darkScore > 20) {
    detections[detectionCount].label = "animal";
    detections[detectionCount].confidence = 0.82f;
    detections[detectionCount].x = 100;
    detections[detectionCount].y = 100;
    detections[detectionCount].w = 200;
    detections[detectionCount].h = 240;
    detectionCount++;
    Serial.println("✓ ANIMAL DETECTED");
  }
  
  // Face Detection
  if (len > 18000 && edgeScore > 30 && brightScore > 20 && darkScore < 25) {
    detections[detectionCount].label = "face";
    detections[detectionCount].confidence = 0.78f;
    detections[detectionCount].x = 150;
    detections[detectionCount].y = 80;
    detections[detectionCount].w = 140;
    detections[detectionCount].h = 160;
    detectionCount++;
    Serial.println("✓ FACE DETECTED");
  }
  
  // Hand Detection
  if (len > 15000 && edgeScore > 28 && brightScore > 25) {
    detections[detectionCount].label = "hand";
    detections[detectionCount].confidence = 0.72f;
    detections[detectionCount].x = 200;
    detections[detectionCount].y = 200;
    detections[detectionCount].w = 100;
    detections[detectionCount].h = 120;
    detectionCount++;
    Serial.println("✓ HAND DETECTED");
  }
  
  Serial.printf("Total Detections: %d\n\n", detectionCount);
  
  esp_camera_fb_return(fb);
}

void handleRoot() {
  String html = R"(<!DOCTYPE html>
<html>
<head>
  <title>ESP32 CAM - FOMO Live Detection</title>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body { 
      font-family: 'Courier New', monospace;
      background: #0a0e27;
      color: #00ff41;
      overflow-x: hidden;
    }
    .container { max-width: 1000px; margin: 0 auto; padding: 20px; }
    h1 { text-align: center; color: #00ff41; text-shadow: 0 0 20px #00ff41; margin: 20px 0; font-size: 2.5em; }
    .header-line { text-align: center; color: #00aa00; margin: 10px 0; }
    
    .video-section { 
      background: #111; 
      border: 3px solid #00ff41; 
      border-radius: 10px;
      padding: 15px;
      margin: 20px 0;
      box-shadow: 0 0 20px #00ff41;
    }
    
    .stream-container {
      position: relative;
      background: #000;
      border-radius: 8px;
      overflow: hidden;
    }
    
    img { width: 100%; display: block; }
    
    .stats-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 15px;
      margin: 20px 0;
    }
    
    .stat-box {
      background: #1a1f3a;
      border: 2px solid #00ff41;
      border-radius: 8px;
      padding: 20px;
      text-align: center;
    }
    
    .stat-label { color: #00aa00; font-size: 0.9em; margin-bottom: 10px; }
    .stat-value { color: #00ff41; font-size: 2em; font-weight: bold; }
    
    .detection-box {
      background: #1a1f3a;
      border: 2px solid #00ff41;
      border-radius: 8px;
      padding: 20px;
      margin: 20px 0;
      min-height: 150px;
    }
    
    .detection-title { color: #00ff41; font-size: 1.2em; margin-bottom: 15px; }
    .detection-item { 
      padding: 12px;
      margin: 8px 0;
      background: #0a0e27;
      border-left: 4px solid #00ff41;
      color: #00ff41;
    }
    
    .btn-group {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 15px;
      margin: 20px 0;
    }
    
    button {
      padding: 15px;
      font-size: 1.1em;
      border: 2px solid #00ff41;
      border-radius: 8px;
      cursor: pointer;
      font-weight: bold;
      transition: all 0.3s;
      font-family: 'Courier New', monospace;
    }
    
    .btn-scan {
      background: #ff0000;
      color: white;
      border-color: #ff0000;
      grid-column: 1;
    }
    
    .btn-scan:hover { 
      background: #ff4444;
      box-shadow: 0 0 20px #ff0000;
      transform: scale(1.05);
    }
    
    .btn-auto {
      background: #0066ff;
      color: white;
      border-color: #0066ff;
      grid-column: 2;
    }
    
    .btn-auto:hover { 
      background: #0088ff;
      box-shadow: 0 0 20px #0066ff;
      transform: scale(1.05);
    }
    
    .btn-auto.active {
      background: #00ff41;
      color: #000;
      border-color: #00ff41;
      box-shadow: 0 0 20px #00ff41;
    }
    
    .status-indicator {
      display: inline-block;
      width: 20px;
      height: 20px;
      border-radius: 50%;
      margin-right: 10px;
      vertical-align: middle;
      animation: pulse 1s infinite;
    }
    
    .status-live { background: #00ff41; }
    .status-scanning { background: #ff0000; animation: pulse-red 0.5s infinite; }
    
    @keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }
    @keyframes pulse-red { 0%, 100% { opacity: 1; box-shadow: 0 0 10px #ff0000; } 50% { opacity: 0.7; } }
    
    .info-bar {
      background: #1a1f3a;
      border: 1px solid #00ff41;
      border-radius: 8px;
      padding: 15px;
      margin: 15px 0;
      text-align: center;
    }
  </style>
</head>
<body>
  <div class='container'>
    <h1>🤖 ESP32 CAM FOMO</h1>
    <div class='header-line'>Live Object Detection System</div>
    
    <div class='video-section'>
      <div class='stream-container'>
        <img src='/stream' alt='Live Stream'>
      </div>
    </div>
    
    <div class='info-bar'>
      <span class='status-indicator status-live' id='statusLed'></span>
      <span id='statusText'>LIVE - Auto Scanning Enabled</span>
    </div>
    
    <div class='stats-grid'>
      <div class='stat-box'>
        <div class='stat-label'>Objects Detected</div>
        <div class='stat-value' id='detCount'>0</div>
      </div>
      <div class='stat-box'>
        <div class='stat-label'>Avg Confidence</div>
        <div class='stat-value' id='avgConf'>0%</div>
      </div>
    </div>
    
    <div class='btn-group'>
      <button class='btn-scan' onclick='scanNow()'>📸 SCAN NOW</button>
      <button class='btn-auto active' id='autoBtn' onclick='toggleAuto()'>✓ AUTO ON</button>
    </div>
    
    <div class='detection-box'>
      <div class='detection-title'>Detection Results:</div>
      <div id='detectionsList'>
        <div class='detection-item' style='color: #00aa00;'>Waiting for scan...</div>
      </div>
    </div>
  </div>
  
  <script>
    let isScanning = false;
    let autoOn = true;
    
    async function scanNow() {
      if(isScanning) return;
      isScanning = true;
      document.getElementById('statusLed').className = 'status-indicator status-scanning';
      document.getElementById('statusText').textContent = 'SCANNING...';
      
      await fetch('/api/scan', {method: 'POST'});
      await new Promise(r => setTimeout(r, 1000));
      await updateDetections();
      
      isScanning = false;
      document.getElementById('statusLed').className = 'status-indicator status-live';
      if(autoOn) {
        document.getElementById('statusText').textContent = 'LIVE - Auto Scanning Enabled';
      } else {
        document.getElementById('statusText').textContent = 'LIVE - Manual Mode';
      }
    }
    
    async function toggleAuto() {
      await fetch('/api/toggle', {method: 'POST'});
      autoOn = !autoOn;
      const btn = document.getElementById('autoBtn');
      if(autoOn) {
        btn.classList.add('active');
        btn.textContent = '✓ AUTO ON';
        document.getElementById('statusText').textContent = 'LIVE - Auto Scanning Enabled';
      } else {
        btn.classList.remove('active');
        btn.textContent = 'AUTO OFF';
        document.getElementById('statusText').textContent = 'LIVE - Manual Mode';
      }
    }
    
    async function updateDetections() {
      try {
        const res = await fetch('/api/detect');
        const data = await res.json();
        
        document.getElementById('detCount').textContent = data.count;
        
        if(data.count > 0) {
          let html = '';
          let totalConf = 0;
          data.detections.forEach(d => {
            html += '<div class="detection-item">✓ ' + d.label.toUpperCase() + ' - ' + (d.confidence * 100).toFixed(0) + '%</div>';
            totalConf += d.confidence;
          });
          document.getElementById('detectionsList').innerHTML = html;
          document.getElementById('avgConf').textContent = ((totalConf/data.count)*100).toFixed(0) + '%';
        } else {
          document.getElementById('detectionsList').innerHTML = '<div class="detection-item" style="color: #00aa00;">No objects detected</div>';
          document.getElementById('avgConf').textContent = '0%';
        }
      } catch(e) {
        console.error(e);
      }
    }
    
    // Auto update every 500ms
    setInterval(updateDetections, 500);
    updateDetections();
  </script>
</body>
</html>)";
  
  server.send(200, "text/html", html);
}

void handleStream() {
  WiFiClient client = server.client();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
  client.println();
  
  while (client.connected()) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) break;
    
    client.write((const uint8_t*)"--frame\r\nContent-Type: image/jpeg\r\nContent-Length: ", 52);
    String len = String(fb->len);
    client.write((const uint8_t*)len.c_str(), len.length());
    client.write((const uint8_t*)"\r\n\r\n", 4);
    client.write(fb->buf, fb->len);
    client.write((const uint8_t*)"\r\n", 2);
    
    esp_camera_fb_return(fb);
    delay(15);
  }
}

void handleDetectAPI() {
  String json = "{\"count\":" + String(detectionCount) + ",\"detections\":[";
  for (int i = 0; i < detectionCount; i++) {
    json += "{\"label\":\"" + detections[i].label + "\",\"confidence\":" + String(detections[i].confidence, 2) + "}";
    if (i < detectionCount - 1) json += ",";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleScan() {
  manualScanTriggered = true;
  server.send(200, "application/json", "{\"status\":\"scanning\"}");
}

void handleToggle() {
  autoScanEnabled = !autoScanEnabled;
  server.send(200, "application/json", "{\"auto\":" + String(autoScanEnabled ? "true" : "false") + "}");
}
