# Check if running as administrator and request elevation if needed
if (-NOT ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "Requesting administrator privileges..."
    Start-Process PowerShell -ArgumentList "-ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    exit
}

Write-Host "Running with administrator privileges..." -ForegroundColor Green

try {
    # Allow incoming UDP packets on port 9999
    Write-Host "Adding UDP 9999 incoming rule..." -ForegroundColor Yellow
    netsh advfirewall firewall add rule name="UDP 9999 In" dir=in action=allow protocol=UDP localport=9999
    
    # Allow outgoing UDP packets on port 9999
    Write-Host "Adding UDP 9999 outgoing rule..." -ForegroundColor Yellow
    netsh advfirewall firewall add rule name="UDP 9999 Out" dir=out action=allow protocol=UDP localport=9999

    # Allow incoming TCP packets on port 8888
    Write-Host "Adding TCP 8888 incoming rule..." -ForegroundColor Yellow
    netsh advfirewall firewall add rule name="TCP 8888 In" dir=in action=allow protocol=TCP localport=8888
    
    # Allow outgoing TCP packets on port 8888
    Write-Host "Adding TCP 8888 outgoing rule..." -ForegroundColor Yellow
    netsh advfirewall firewall add rule name="TCP 8888 Out" dir=out action=allow protocol=TCP localport=8888

    Write-Host "Firewall rules configured successfully!" -ForegroundColor Green
}
catch {
    Write-Host "Error occurred: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host "Press any key to close this window..." -ForegroundColor Cyan
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
