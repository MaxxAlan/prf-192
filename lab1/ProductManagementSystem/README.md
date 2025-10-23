# Product Management System (PMS) v1.0

**Group 3 – Lab 1, PRF192 – FPT University**

A console-based **Product Management System** written in **pure C (C11)** for **Windows 11**, developed and compiled using **Embarcadero Dev-C++ 6.3** with **TDM-GCC 9.2.0**.

## Overview

This project simulates a **Product Management System** with a hierarchical structure:

- Category → Subgroup → Product
  Each level supports CRUD operations, binary file persistence, and statistical reports.

### Main Features

- CRUD for categories, subgroups, and products
- Binary file storage (`products.dat`) with backup
- Search by name, price range, or quantity
- Statistical summaries (totals, averages, values)
- Auto-increment ID and timestamp tracking
- Input validation and memory safety

## Learning Objectives (Lab 1 – PRF192)

- Apply modular programming with header/source separation
- Use binary file handling for persistent storage
- Implement dynamic memory allocation and structs
- Strengthen function, loop, and array manipulation
- Practice teamwork and version control

## Development Environment

| Component         | Version / Detail        |
| ----------------- | ----------------------- |
| IDE               | Embarcadero Dev-C++ 6.3 |
| Compiler          | TDM-GCC 9.2.0 (MinGW64) |
| Language Standard | C11                     |
| OS                | Windows 11              |
| Encoding          | UTF-8                   |
| Line Ending       | CRLF                    |

## Project Structure

```
ProductManagementSystem/
│
├── include/
│   ├── product.h
│   ├── subgroup.h
│   ├── category.h
│   └── utils.h
│
├── src/
│   ├── main.c
│   ├── product.c
│   ├── subgroup.c
│   ├── category.c
│   └── utils.c
│
├── data/
│   ├── products.dat
│   └── products.bak
│
├── obj/
├── ProductManagementSystem.dev
├── Makefile.win
└── README.md
```

## Build and Run

### Using Dev-C++ (Recommended)

1. Open `ProductManagementSystem.dev`
2. Build → Rebuild All (Ctrl+F11)
3. Run → Execute (F10)

### Using Command Line

```cmd
set PATH=C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin;%PATH%
mkdir obj
mingw32-make -f Makefile.win
ProductManagementSystem.exe
```

## Usage

### Main Menu

```
[1] Category Management
[2] Subgroup Management
[3] Product Management
[4] Search & Filter
[5] Statistics & Reports
[6] View All Data
[0] Save & Exit
```

### Workflow

1. Add Categories
2. Add Subgroups to each category
3. Add Products to subgroups
4. Use Search or Statistics
5. Save and exit (auto-backup enabled)

## Statistics and Data Management

- Total categories, subgroups, and products
- Total inventory quantity and value
- Average product price
- Data stored in binary (`data/products.dat`)

## Error Handling

- Input validation and range checking
- File I/O protection and recovery
- Safe dynamic memory allocation
- Auto-backup before overwrite

## Technical Notes

- Binary storage for efficiency
- Dynamic arrays with capacity doubling
- All memory freed on exit
- Screen clear command: `cls`
- Maximum lengths:

  - Category/Subgroup name: 50
  - Product name: 100
  - Description: 200

## Future Improvements

- CSV/Excel export
- Multi-user access
- SQLite backend
- Stock alerts and supplier management

## Group 3 Members – FPT University (PRF192, Lab 1)

| No  | Full Name           | Student ID | Role   |
| --- | ------------------- | ---------- | ------ |
| 1   | Hoàng Tiến Mạnh     | SE203727   | Leader |
| 2   | Đặng Thị Phương Anh | SE2009     | Member |
| 3   | Võ Đặng Anh Khoa    | SE2009     | Member |
| 4   | Dương Lê Tuấn Anh   | SE2009     | Member |
| 5   | Tôn Thị Thu Phương  | SE2009     | Member |

---

## License

For educational use only – PRF192, FPT University.

## Version

v1.0 – 2025
Compatible with: Dev-C++ 6.3, TDM-GCC 9.2.0, Windows 11
