@echo off
REM Run script for Product Management System

if not exist ProductManagementSystem.exe (
    echo ERROR: ProductManagementSystem.exe not found!
    echo Please build the project first using build.bat
    pause
    exit /b 1
)

REM Ensure data directory exists
if not exist data mkdir data

REM Set UTF-8 code page for proper character display
chcp 65001 >nul 2>nul

cls
echo Starting Product Management System...
echo.

ProductManagementSystem.exe

echo.
echo Program terminated.
pause