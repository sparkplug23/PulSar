@echo off
setlocal
cd /d "%~dp0"
echo Installing zeroconf for the Python interpreter used to launch the designer.
where py >nul 2>nul
if %errorlevel%==0 (
    py -3.15 -m pip install zeroconf
    if errorlevel 1 py -m pip install zeroconf
) else (
    python -m pip install zeroconf
)
echo.
pause
