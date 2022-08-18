@echo off
mkdir ..\build
pushd %~dp0\..\build\
call cmake ..
popd
PAUSE