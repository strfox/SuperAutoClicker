param(
    [Parameter(Mandatory=$True)][string]$ExePath,
    [string]$QtBinDir = "C:\Qt\5.12.3\msvc2017\bin",
    [string[]]$Passthrough
)

$ExeDir = (Get-Item $ExePath).Directory

$ErrorActionPreference = "Inquire"
& "$QtBinDir\windeployqt.exe" --release `
        --no-quick-import `
        --no-translations `
        --no-system-d3d-compiler `
        --no-compiler-runtime `
        --no-webkit2 `
        --no-angle `
        --no-opengl-sw `
        --no-svg `
        --verbose 1 `
        $Passthrough $ExePath

# Delete unnecessary image formats
$ImageFormatsDir = Join-Path -Path $ExeDir -ChildPath "imageformats"

$ErrorActionPreference = "Stop"
Remove-Item -Path "$ImageFormatsDir\*" `
        -Verbose `
        -Include qgif.dll,  qincs.dll, qjpeg.dll,
                 qtga.dll,  qtiff.dll, qwbmp.dll,
                 qwebp.dll, qicns.dll