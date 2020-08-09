@echo off
pushd %~dp0\..\AssetBase
call ..\build\Release-windows-x86_64\AssetBase\AssetBase.exe
setlocal
:PROMPT
SET /P DOCOPY="Write lab files to Horizons/data/ ? (Y/[N]) "
IF /I "%DOCOPY%" NEQ "Y" GOTO END

copy .\output\*.* ..\Horizons\data\
PAUSE

:END
endlocal
popd