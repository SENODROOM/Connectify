@echo off
cd /d C:\Programming\Connectify\build

echo Building Connectify...
mingw32-make -j4

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo BUILD FAILED. Check errors above.
    pause
    exit /b 1
)

echo.
echo Build successful! Launching Connectify...
echo.
.\Connectify.exe