@echo off
REM Set UTF-8 encoding for proper display
chcp 65001 >nul

echo ========================================
echo Building Product Management System
echo Dev-C++ 6.3 - TDM-GCC 9.2.0
echo ========================================
echo.

REM Create directories
if not exist obj mkdir obj
if not exist data mkdir data

REM Compiler path
set GCC="C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc"

REM Compiler flags (added -fexec-charset=UTF-8 for proper encoding)
set CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2 -g -Iinclude -fexec-charset=UTF-8

echo Compiling source files...
echo.

REM Compile each module
echo [1/5] Compiling product.c...
%GCC% %CFLAGS% -c src/product.c -o obj/product.o
if %errorlevel% neq 0 goto :error

echo [2/5] Compiling subgroup.c...
%GCC% %CFLAGS% -c src/subgroup.c -o obj/subgroup.o
if %errorlevel% neq 0 goto :error

echo [3/5] Compiling category.c...
%GCC% %CFLAGS% -c src/category.c -o obj/category.o
if %errorlevel% neq 0 goto :error

echo [4/5] Compiling utils.c...
%GCC% %CFLAGS% -c src/utils.c -o obj/utils.o
if %errorlevel% neq 0 goto :error

echo [5/5] Compiling main.c...
%GCC% %CFLAGS% -c src/main.c -o obj/main.o
if %errorlevel% neq 0 goto :error

echo.
echo Linking objects...

REM Link all object files
%GCC% obj/product.o obj/subgroup.o obj/category.o obj/utils.o obj/main.o ^
      -o ProductManagementSystem.exe -static-libgcc

if %errorlevel% neq 0 goto :error

REM Success message
echo.
echo ========================================
echo ✓ Build successful!
echo ========================================
echo.
echo Output: ProductManagementSystem.exe
echo.
echo To run with proper colors and UTF-8:
echo   run.bat
echo.
goto :end

:error
echo.
echo ========================================
echo ✗ Build failed! Check errors above.
echo ========================================
echo.
goto :end

:end
pause