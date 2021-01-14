param (
	[switch]$noprompt = $false
)

Push-Location "${PSScriptRoot}\..\"
bin\premake\premake5.exe vs2019
Pop-Location

if (-Not $noprompt)
{
	Write-Host "Press any key to finish...";
	$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown");
}
