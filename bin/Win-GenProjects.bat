@echo off
pushd %~dp0\..\
call bin\premake\premake5.exe vs2019
popd
PAUSE