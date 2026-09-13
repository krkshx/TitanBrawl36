@echo off
rem TitanBrawl36 — собрать и запустить (аналог run.sh под Windows).
setlocal
cd /d "%~dp0"

call "%~dp0build.bat" %*
if %ERRORLEVEL% neq 0 exit /b 1

build\titan_pc.exe .
