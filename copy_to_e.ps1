param (
    [string]$SourceFile,
    [string]$DestinationDrive
)

Copy-Item -Path $SourceFile -Destination "$DestinationDrive\main.bin" -Force