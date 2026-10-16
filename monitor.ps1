$port = New-Object System.IO.Ports.SerialPort
$port.PortName = "COM4"
$port.BaudRate = 115200
$port.Open()

Write-Host "Waiting for ESP32 startup (30 seconds)..." -ForegroundColor Green

$startTime = Get-Date
$ipFound = $false

while ((Get-Date) - $startTime -lt (New-TimeSpan -Seconds 30)) {
    try {
        $char = $port.ReadChar()
        if ($char -eq 10) {  # newline
            Write-Host ""
        } else {
            Write-Host ([char]$char) -NoNewline
            if ($char -gt 31 -and $char -lt 127) {
                # Track if we found IP
                if ($char -eq "1" -or $char -eq "2" -or $char -eq "0") {
                    $ipFound = $true
                }
            }
        }
    } catch {}
}

$port.Close()
Write-Host "`n`nSerial monitoring complete!`n" -ForegroundColor Yellow
Write-Host "If you see 'IP address: xxx.xxx.xxx.xxx', use that address in your browser" -ForegroundColor Green
