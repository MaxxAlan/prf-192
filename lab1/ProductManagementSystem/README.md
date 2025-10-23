# Product Management System (PMS) v1.0

A comprehensive console-based Product Management System written in pure C (C11 standard) for Windows 11, developed using Embarcadero Dev-C++ 6.3 with TDM-GCC 9.2.0 compiler.

## 📋 Overview

The Product Management System provides a hierarchical structure for managing:

- **Categories** - Top-level organizational units
- **Subgroups** - Mid-level groupings within categories
- **Products** - Individual items with detailed information

### Key Features

✅ **Complete CRUD Operations** - Create, Read, Update, Delete for all entities  
✅ **Hierarchical Data Structure** - Categories → Subgroups → Products  
✅ **Binary File Persistence** - Data stored in `products.dat` with automatic backup  
✅ **Search & Filter** - Find products by name, price range, or quantity  
✅ **Statistics & Reports** - View totals, averages, and inventory value  
✅ **Auto-incrementing IDs** - Unique identifiers for all entities  
✅ **Input Validation** - Comprehensive error checking and safe input handling  
✅ **Dynamic Memory Management** - Efficient array resizing as data grows  
✅ **Timestamp Tracking** - Created/updated timestamps for all products

## 🎯 Development Environment

- **IDE**: Embarcadero Dev-C++ 6.3
- **Compiler**: TDM-GCC 9.2.0 (MinGW64)
- **C Standard**: C11
- **Target OS**: Windows 11
- **Console**: Windows Command Prompt
- **Encoding**: UTF-8
- **Line Endings**: CRLF (Windows)

## 📁 Project Structure

```
ProductManagementSystem/
│
├── ProductManagementSystem.dev    # Dev-C++ project file
├── Makefile.win                   # Build configuration
├── README.md                      # This file
│
├── include/                       # Header files
│   ├── product.h
│   ├── subgroup.h
│   ├── category.h
│   └── utils.h
│
├── src/                           # Source files
│   ├── main.c
│   ├── product.c
│   ├── subgroup.c
│   ├── category.c
│   └── utils.c
│
├── obj/                           # Object files (auto-created)
│   └── *.o
│
├── data/                          # Data files
│   ├── products.dat               # Binary data file
│   └── products.bak               # Automatic backup
│
└── ProductManagementSystem.exe    # Executable (after build)
```

## 🚀 Setup Instructions

### Prerequisites

1. **Install Embarcadero Dev-C++ 6.3**

   - Download from official Embarcadero website
   - Ensure TDM-GCC 9.2.0 is included

2. **Create Data Directory**
   ```bash
   mkdir data
   ```

### Building the Project

#### Method 1: Using Dev-C++ IDE (Recommended)

1. Open `ProductManagementSystem.dev` in Dev-C++ 6.3
2. Go to **Execute → Compile** (F9) or **Execute → Rebuild All** (Ctrl+F11)
3. If successful, `ProductManagementSystem.exe` will be created
4. Run with **Execute → Run** (F10) or directly from Explorer

#### Method 2: Using Command Line

1. Open Command Prompt in the project directory
2. Ensure MinGW/TDM-GCC is in your PATH:

   ```cmd
   set PATH=C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin;%PATH%
   ```

3. Create obj directory if it doesn't exist:

   ```cmd
   mkdir obj
   ```

4. Build using the Makefile:

   ```cmd
   mingw32-make -f Makefile.win
   ```

5. Run the executable:
   ```cmd
   ProductManagementSystem.exe
   ```

#### Method 3: Manual Compilation

```cmd
gcc -c src/product.c -o obj/product.o -std=c11 -Wall -Iinclude
gcc -c src/subgroup.c -o obj/subgroup.o -std=c11 -Wall -Iinclude
gcc -c src/category.c -o obj/category.o -std=c11 -Wall -Iinclude
gcc -c src/utils.c -o obj/utils.o -std=c11 -Wall -Iinclude
gcc -c src/main.c -o obj/main.o -std=c11 -Wall -Iinclude
gcc obj/main.o obj/product.o obj/subgroup.o obj/category.o obj/utils.o -o ProductManagementSystem.exe -static-libgcc
```

## 📖 Usage Guide

### Main Menu

When you run the application, you'll see:

```
╔════════════════════════════════════════════════════════════╗
║        PRODUCT MANAGEMENT SYSTEM v1.0                      ║
║        Dev-C++ 6.3 - Windows 11                           ║
╚════════════════════════════════════════════════════════════╝

  [1] Category Management
  [2] Subgroup Management
  [3] Product Management
  [4] Search & Filter
  [5] Statistics & Reports
  [6] View All Data
  [0] Save & Exit
```

### Typical Workflow

1. **Create Categories First**

   - Navigate to Category Management → Add Category
   - Enter category name and description

2. **Add Subgroups to Categories**

   - Navigate to Subgroup Management → Add Subgroup
   - Select parent category
   - Enter subgroup details

3. **Add Products to Subgroups**

   - Navigate to Product Management → Add Product
   - Select parent subgroup
   - Enter product details (code, name, price, quantity)

4. **Search and Analyze**

   - Use Search & Filter to find specific products
   - View Statistics & Reports for inventory insights

5. **Save Data**
   - Choose option [0] to save and exit
   - Data is automatically backed up to `products.bak`

### Data Management

- **Auto-save on Exit**: If data is modified, you'll be prompted to save
- **Automatic Backup**: Previous data file is backed up before each save
- **Binary Format**: Efficient storage in `data/products.dat`
- **Data Persistence**: All data loads automatically on startup

### Search Capabilities

- **By Name**: Partial, case-insensitive text matching
- **By Price Range**: Find products within min/max price
- **By Quantity Range**: Filter by stock levels

### Statistics Provided

- Total number of categories, subgroups, products
- Total inventory quantity
- Total inventory value (price × quantity)
- Average product price

## 🔧 Technical Details

### Data Structures

- **Dynamic Arrays**: All collections use dynamic memory with automatic resizing
- **Initial Capacity**: 10 items per collection
- **Growth Strategy**: Capacity doubles when full (10 → 20 → 40...)
- **Memory Management**: Proper cleanup with cascading free operations

### File Format

Binary file structure:

1. Header: category count, next ID values
2. For each category:
   - Category data (ID, name, description)
   - Subgroup count
   - For each subgroup:
     - Subgroup data
     - Product count
     - Product array

### Error Handling

- Input validation on all user entries
- NULL pointer checks throughout
- File I/O error detection
- Memory allocation failure handling
- Graceful degradation on errors

### Compiler Flags

- **Debug Build**: `-std=c11 -Wall -Wextra -pedantic -g`
- **Release Build**: `-std=c11 -Wall -Wextra -pedantic -O2`
- **Linker**: `-static-libgcc` for standalone executable

## 🐛 Troubleshooting

### Build Errors

**Problem**: `cannot find -lmingw32` or similar linker errors  
**Solution**: Check that TDM-GCC bin directory is in your PATH

**Problem**: `undefined reference` errors  
**Solution**: Ensure all `.c` files are compiled and linked

**Problem**: Header files not found  
**Solution**: Verify `-Iinclude` flag is used during compilation

### Runtime Errors

**Problem**: "Cannot open file data/products.dat"  
**Solution**: Create the `data` directory in project root

**Problem**: Data not saving  
**Solution**: Check write permissions in the `data` directory

**Problem**: Crash on startup  
**Solution**: Delete corrupted `products.dat` and restart with empty database

### Common Issues

- **Screen not clearing**: Ensure running in Windows Command Prompt (not PowerShell)
- **Garbage characters**: Set console to UTF-8: `chcp 65001`
- **Memory leaks**: All memory is freed on exit; use valgrind alternative on Windows if needed

## 📝 Notes

- **No External Dependencies**: Uses only standard C libraries
- **Windows-Specific**: Uses `cls` for screen clearing
- **Single-User**: Not designed for concurrent access
- **UTF-8 Support**: Handles international characters in product names
- **Maximum Sizes**:
  - Category/Subgroup name: 50 chars
  - Product name: 100 chars
  - Product code: 20 chars
  - Descriptions: 200 chars

## 🔄 Future Enhancements

Potential improvements for future versions:

- Export to CSV/Excel
- Import from external files
- Multi-user support with access control
- Database backend (SQLite)
- Barcode integration
- Low stock alerts
- Sales tracking
- Supplier management

## 👥 Authors

**PMS Team** - Product Management System Development

## 📄 License

This project is developed for educational purposes using Embarcadero Dev-C++ 6.3.

## 🆘 Support

For issues or questions:

1. Check the Troubleshooting section
2. Review code comments in source files
3. Verify build environment matches specifications
4. Test with a clean build (`mingw32-make clean` then rebuild)

---

**Version**: 1.0  
**Last Updated**: 2025  
**Compatible With**: Embarcadero Dev-C++ 6.3, TDM-GCC 9.2.0, Windows 11
