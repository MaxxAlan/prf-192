@echo off
REM ========================================
REM Product Management System Launcher
REM Ensures proper UTF-8 encoding & colors
REM ========================================

REM Set console to UTF-8 encoding
chcp 65001 >nul

REM Set console size (optional)
mode con: cols=120 lines=40

REM Clear screen
cls

echo ========================================
echo   Product Management System v1.0
echo   Starting with UTF-8 encoding...
echo ========================================
echo.

REM Check if data directory exists
if not exist "data" (
    echo [WARNING] Creating data directory...
    mkdir data
    echo.
)

REM Run the program
ProductManagementSystem.exe

REM Restore code page (optional)
REM chcp 1252 >nul

pause