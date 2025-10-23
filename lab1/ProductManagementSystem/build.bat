@echo off
echo ========================================
echo Building Product Management System
echo Dev-C++ 6.3 - TDM-GCC 9.2.0
echo ========================================

REM Create directories
if not exist obj mkdir obj
if not exist data mkdir data

REM Compile source files with full path to gcc
"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc" -std=c11 -Wall -Wextra -pedantic -O2 -g -Iinclude -c src/product.c -o obj/product.o
"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc" -std=c11 -Wall -Wextra -pedantic -O2 -g -Iinclude -c src/subgroup.c -o obj/subgroup.o
"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc" -std=c11 -Wall -Wextra -pedantic -O2 -g -Iinclude -c src/category.c -o obj/category.o
"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc" -std=c11 -Wall -Wextra -pedantic -O2 -g -Iinclude -c src/utils.c -o obj/utils.o
"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc" -std=c11 -Wall -Wextra -pedantic -O2 -g -Iinclude -c src/main.c -o obj/main.o

REM Link
"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\gcc" obj/product.o obj/subgroup.o obj/category.o obj/utils.o obj/main.o -o ProductManagementSystem.exe -static-libgcc

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo ========================================
    echo.
    echo Run: ProductManagementSystem.exe
) else (
    echo.
    echo ========================================
    echo Build failed! Check errors above.
    echo ========================================
)

pause