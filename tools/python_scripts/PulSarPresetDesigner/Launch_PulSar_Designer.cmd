@echo off
setlocal
cd /d "%~dp0"
set "UV_PY=%APPDATA%\uv\python\cpython-3.15.1-windows-x86_64-none\python.exe"
if exist "%UV_PY%" (
    "%UV_PY%" main.py
) else (
    py -3.15 main.py
)
if errorlevel 1 pause
