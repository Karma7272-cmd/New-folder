// State variables
let deviceAddress = localStorage.getItem('esp32_ip') || '';
let isDemoMode = true;
let isAutoScan = true;
let isFlashOn = false;
let autoScanInterval = null;
let simulatedDetections = [];

// DOM Elements
const deviceIpInput = document.getElementById('deviceIp');
const connectBtn = document.getElementById('connectBtn');
const connectionStatus = document.getElementById('connectionStatus');
const statusText = document.getElementById('statusText');
const streamImage = document.getElementById('streamImage');
const canvas = document.getElementById('overlayCanvas');
const ctx = canvas.getContext('2d');
const modeDemoBtn = document.getElementById('modeDemoBtn');
const modeLiveBtn = document.getElementById('modeLiveBtn');
const scanBtn = document.getElementById('scanBtn');
const autoToggleBtn = document.getElementById('autoToggleBtn');
const flashToggleBtn = document.getElementById('flashToggleBtn');
const detCount = document.getElementById('detCount');
const avgConfidence = document.getElementById('avgConfidence');
const detectionsList = document.getElementById('detectionsList');
const lastUpdated = document.getElementById('lastUpdated');
const fpsCounter = document.getElementById('fpsCounter');

// Initialize App
document.addEventListener('DOMContentLoaded', () => {
  if (deviceAddress) {
    deviceIpInput.value = deviceAddress;
  }

  setupEventListeners();
  resizeCanvas();
  window.addEventListener('resize', resizeCanvas);

  // Start initial auto scan in demo mode
  if (isAutoScan) {
    startAutoScan();
  }
});

function setupEventListeners() {
  connectBtn.addEventListener('click', handleConnect);
  modeDemoBtn.addEventListener('click', () => setMode(true));
  modeLiveBtn.addEventListener('click', () => setMode(false));
  scanBtn.addEventListener('click', triggerScan);
  autoToggleBtn.addEventListener('click', toggleAutoScan);
  flashToggleBtn.addEventListener('click', toggleFlash);
}

function resizeCanvas() {
  canvas.width = streamImage.clientWidth || 640;
  canvas.height = streamImage.clientHeight || 480;
  drawDetections(simulatedDetections);
}

function setMode(demo) {
  isDemoMode = demo;
  if (isDemoMode) {
    modeDemoBtn.classList.add('active');
    modeLiveBtn.classList.remove('active');
    statusText.textContent = 'Demo / Standalone Mode';
    connectionStatus.querySelector('.dot').className = 'dot offline';
    streamImage.src = 'https://images.unsplash.com/photo-1526374965328-7f61d4dc18c5?auto=format&fit=crop&w=800&q=80';
  } else {
    if (!deviceAddress) {
      alert('Please enter an ESP32 device IP address first!');
      setMode(true);
      return;
    }
    modeLiveBtn.classList.add('active');
    modeDemoBtn.classList.remove('active');
    statusText.textContent = `Connected: ${deviceAddress}`;
    connectionStatus.querySelector('.dot').className = 'dot online';
    const baseUrl = deviceAddress.startsWith('http') ? deviceAddress : `http://${deviceAddress}`;
    streamImage.src = `${baseUrl}/stream`;
  }
  triggerScan();
}

function handleConnect() {
  const ip = deviceIpInput.value.trim();
  if (!ip) {
    alert('Please enter a valid IP address or hostname.');
    return;
  }
  deviceAddress = ip;
  localStorage.setItem('esp32_ip', deviceAddress);
  setMode(false);
}

function toggleAutoScan() {
  isAutoScan = !isAutoScan;
  if (isAutoScan) {
    autoToggleBtn.classList.add('active');
    autoToggleBtn.textContent = '✓ AUTO SCAN: ON';
    startAutoScan();
  } else {
    autoToggleBtn.classList.remove('active');
    autoToggleBtn.textContent = 'AUTO SCAN: OFF';
    stopAutoScan();
  }
}

function startAutoScan() {
  stopAutoScan();
  autoScanInterval = setInterval(() => {
    triggerScan();
  }, 1200);
}

function stopAutoScan() {
  if (autoScanInterval) {
    clearInterval(autoScanInterval);
    autoScanInterval = null;
  }
}

async function triggerScan() {
  lastUpdated.textContent = 'Updating...';
  let detections = [];

  if (isDemoMode) {
    // Generate realistic simulated FOMO detections
    detections = generateDemoDetections();
  } else {
    try {
      const baseUrl = deviceAddress.startsWith('http') ? deviceAddress : `http://${deviceAddress}`;
      const res = await fetch(`${baseUrl}/api/detect`);
      const data = await res.json();
      detections = data.detections || [];
    } catch (e) {
      console.warn('Unable to reach ESP32 endpoint, falling back to local simulation:', e);
      detections = generateDemoDetections();
    }
  }

  simulatedDetections = detections;
  updateUI(detections);
  drawDetections(detections);
  lastUpdated.textContent = `Updated ${new Date().toLocaleTimeString()}`;
}

function generateDemoDetections() {
  const classes = [
    { label: 'person', w: 180, h: 260 },
    { label: 'object', w: 140, h: 140 },
    { label: 'face', w: 120, h: 140 },
    { label: 'hand', w: 100, h: 100 }
  ];

  const count = Math.floor(Math.random() * 3) + 1;
  const list = [];

  for (let i = 0; i < count; i++) {
    const cls = classes[Math.floor(Math.random() * classes.length)];
    const confidence = parseFloat((0.65 + Math.random() * 0.32).toFixed(2));
    const x = Math.floor(50 + Math.random() * 300);
    const y = Math.floor(50 + Math.random() * 200);

    list.push({
      label: cls.label,
      confidence: confidence,
      x: x,
      y: y,
      w: cls.w,
      h: cls.h
    });
  }
  return list;
}

function updateUI(detections) {
  detCount.textContent = detections.length;

  if (detections.length > 0) {
    const sumConf = detections.reduce((acc, d) => acc + d.confidence, 0);
    const avg = ((sumConf / detections.length) * 100).toFixed(0);
    avgConfidence.textContent = `${avg}%`;

    detectionsList.innerHTML = detections.map(d => `
      <li class="detection-item">
        <span>✓ ${d.label.toUpperCase()}</span>
        <span class="badge badge-info">${(d.confidence * 100).toFixed(0)}% Conf</span>
      </li>
    `).join('');
  } else {
    avgConfidence.textContent = '0%';
    detectionsList.innerHTML = `<li class="empty-state">No objects currently detected.</li>`;
  }

  // Random FPS display simulation
  fpsCounter.textContent = `FPS: ${(12 + Math.random() * 4).toFixed(1)}`;
}

function drawDetections(detections) {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  detections.forEach(d => {
    const scaleX = canvas.width / 640;
    const scaleY = canvas.height / 480;

    const x = (d.x || 100) * scaleX;
    const y = (d.y || 100) * scaleY;
    const w = (d.w || 150) * scaleX;
    const h = (d.h || 150) * scaleY;

    // Draw Box
    ctx.strokeStyle = '#3fb950';
    ctx.lineWidth = 3;
    ctx.strokeRect(x, y, w, h);

    // Label background
    const labelText = `${d.label.toUpperCase()} ${(d.confidence * 100).toFixed(0)}%`;
    ctx.font = 'bold 14px "Fira Code", monospace';
    const textWidth = ctx.measureText(labelText).width;

    ctx.fillStyle = '#3fb950';
    ctx.fillRect(x, y - 24 > 0 ? y - 24 : y, textWidth + 12, 24);

    // Label text
    ctx.fillStyle = '#0d1117';
    ctx.fillText(labelText, x + 6, (y - 24 > 0 ? y - 24 : y) + 17);
  });
}

function toggleFlash() {
  isFlashOn = !isFlashOn;
  if (isFlashOn) {
    flashToggleBtn.classList.add('btn-primary');
    flashToggleBtn.textContent = '⚡ FLASH: ON';
  } else {
    flashToggleBtn.classList.remove('btn-primary');
    flashToggleBtn.textContent = '⚡ FLASH LED';
  }

  if (!isDemoMode && deviceAddress) {
    const baseUrl = deviceAddress.startsWith('http') ? deviceAddress : `http://${deviceAddress}`;
    fetch(`${baseUrl}/api/toggle`, { method: 'POST' }).catch(() => {});
  }
}
