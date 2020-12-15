@echo off
pushd %~dp0\..\AssetBase
call ..\build\Release-windows-x86_64\AssetBase\AssetBase.exe
setlocal
:PROMPT
SET /P DOCOPY="Write asset files to Horizons/data/ ? (y/N) "
IF /I "%DOCOPY%" == "y" GOTO COPY
IF /I "%DOCOPY%" == "Y" GOTO COPY
GOTO END

:COPY
copy .\output\*.* ..\Horizons\data\
PAUSE

:END
endlocal
popd
