param(
    [bool]$Debug,
    [bool]$Clean,
    [string[]]$Passthrough
)

$ErrorActionPreference = "Stop"

$target = If ($debug) { "Debug" } else { "Release" }
$dir = ".\$target\"

if ($clean) {
    Write-Host "Cleaning $dir..."
    Get-ChildItem -Path $dir -Include *.* -File -Recurse | foreach { $_.Delete() }
}

New-Item -ItemType Directory -Force -Path $dir | Out-Null
Set-Location -Path $dir

try {
cmake.exe -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=$target .. 
mingw32-make.exe @Passthrough
} catch [System.Management.Automation.RemoteException] {
  Write-Host 'Could not build: '$_.Exception.Message
} finally {
  Set-Location -Path ..
}