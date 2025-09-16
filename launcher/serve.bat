@echo off
setlocal

set PORT=%1
if "%PORT%"=="" set PORT=5500

set SCRIPT=%~dp0serve.ps1
if not exist "%SCRIPT%" (
  echo [ERR ] 找不到 serve.ps1
  exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%SCRIPT%" -Port %PORT%

endlocal 