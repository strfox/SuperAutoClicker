param(
    [switch]$Debug,
    [switch]$Clean,
    [string[]]$Passthrough
)

$ErrorActionPreference = "Stop"

$Target = If ($Debug) { "Debug" } else { "Release" }
$BuildDir = ".\$Target\"

# Create build directory (ignore if already exists)
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

# Clean build directory if param was passed
if ($Clean) {
    Write-Host "Cleaning $BuildDir..."
    Get-ChildItem -Path $BuildDir -Include *.* -File -Recurse | foreach { $_.Delete() }
}

Set-Location -Path $BuildDir

try {
cmake.exe -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=$Target .. 
mingw32-make.exe @Passthrough
} catch [System.Management.Automation.RemoteException] {
  Write-Host 'Could not build: '$_.Exception.Message
} finally {
  Set-Location -Path ..
}