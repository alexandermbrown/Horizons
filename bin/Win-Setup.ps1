$libav_url = "https://github.com/mcmtroffaes/ffmpeg-msvc-build/releases/download/20210101.0.0/ffmpeg-2021-01-01-0-lgpl21-x64-windows-static-md.7z"
$libav_archive_name = "ffmpeg-2021-01-01-0-lgpl21-x64-windows-static-md"
$libav_build_path = "Lithium\vendor\libav\build"
$libav_archive_path = "$libav_build_path\$libav_archive_name.7z"
$archive_debug_location = "$libav_archive_name\installed\x64-windows-static-md\debug\lib"
$archive_release_location = "$libav_archive_name\installed\x64-windows-static-md\lib"

Push-Location "$PSScriptRoot\.."

# Check if 7-Zip is installed and in PATH.
try {
	Get-Command "7z" -ErrorAction Stop | Out-Null
}
catch {
	Write-Host "[Win-Init.ps1] 7z.exe not found. Install 7-Zip and add it to PATH." -ForegroundColor Red
	Write-Host "[Win-Init.ps1] Script failed. Press any key to finish...";
	$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown");
	EXIT
}

# Create libav directories.
New-Item -ItemType "Directory" -Force -Path "$libav_build_path\Debug-windows-x86_64" | Out-Null
New-Item -ItemType "Directory" -Force -Path "$libav_build_path\Release-windows-x86_64" | Out-Null

# Download libav binaries if not found.
if (-Not (Test-Path -Path $libav_archive_path)) {
	Write-Host "[Win-Init.ps1] Downloading $libav_url..." -ForegroundColor DarkGreen
	$client = New-Object System.Net.WebClient
	# $client.DownloadFile($libav_url, $libav_archive_path)
	$client.DownloadFile($libav_url, "$PSScriptRoot\..\$libav_archive_path")
	Write-Host "[Win-Init.ps1] Saved to $libav_archive_path" -ForegroundColor DarkGreen
}
else {
	Write-Host "[Win-Init.ps1] $libav_archive_name.7z already exists, skipping download." -ForegroundColor DarkGreen
}

Write-Host "[Win-Init.ps1] Extracting $libav_archive_path..." -ForegroundColor DarkGreen
Remove-Item "$libav_build_path\Debug-windows-x86_64\*.lib" -ErrorAction Ignore | Out-Null
Remove-Item "$libav_build_path\Debug-windows-x86_64\*.pdb" -ErrorAction Ignore | Out-Null
Remove-Item "$libav_build_path\Release-windows-x86_64\*.lib" -ErrorAction Ignore | Out-Null
Remove-Item "$libav_build_path\Release-windows-x86_64\*.pdb" -ErrorAction Ignore | Out-Null

7z e $libav_archive_path "-o$libav_build_path\Debug-windows-x86_64" "$archive_debug_location\*.lib" "$archive_debug_location\*.pdb"
7z e $libav_archive_path "-o$libav_build_path\Release-windows-x86_64" "$archive_release_location\*.lib" "$archive_release_location\*.pdb"

# Create build directories.
New-Item -ItemType "Directory" -Force -Path "build\Debug-windows-x86_64\Horizons"
New-Item -ItemType "Directory" -Force -Path "build\Release-windows-x86_64\Horizons"
New-Item -ItemType "Directory" -Force -Path "build\Dist-windows-x86_64\Horizons"

# Copy PDBs
Write-Host "[Win-Init.ps1] Copying PDB files..." -ForegroundColor DarkGreen
Copy-Item "$libav_build_path\Debug-windows-x86_64\*.pdb" -Destination "build\Debug-windows-x86_64\Horizons\"
Copy-Item "$libav_build_path\Release-windows-x86_64\*.pdb" -Destination "build\Release-windows-x86_64\Horizons\"
Copy-Item "$libav_build_path\Release-windows-x86_64\*.pdb" -Destination "build\Dist-windows-x86_64\Horizons\"

# Copy DLLs
$sdl_dll_path = "Lithium\vendor\SDL2\lib\win64\SDL2.dll"
Write-Host "[Win-Init.ps1] Copying DLL files..." -ForegroundColor DarkGreen
Copy-Item $sdl_dll_path -Destination "build\Debug-windows-x86_64\Horizons\"
Copy-Item $sdl_dll_path -Destination "build\Release-windows-x86_64\Horizons\"
Copy-Item $sdl_dll_path -Destination "build\Dist-windows-x86_64\Horizons\"

Write-Host "[Win-Init.ps1] Generating projects..." -ForegroundColor DarkGreen
bin/Win-GenProjects.ps1 -noprompt
Pop-Location

Write-Host "[Win-Init.ps1] Press any key to finish...";
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown");
