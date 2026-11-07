@echo off
setlocal
cd /d "%~dp0"
where py >nul 2>nul
if %errorlevel%==0 (
    py -3.15 main.py
    if not errorlevel 9009 exit /b %errorlevel%
    py main.py
) else (
    python main.py
)
if errorlevel 1 pause
