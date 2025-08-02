# Hostel Management System with SQLite

A hostel management system that uses SQLite database instead of CSV files for better data management and security.

## Quick Start

### 1. Build the Application
```bash
make
```

### 2. Run the Application
```bash
./hostel_sqlite
```

### 3. Build and Run in One Step
```bash
make run
```

## Makefile Commands

- `make` - Build the application
- `make run` - Build and run
- `make clean` - Remove build files and database
- `make help` - Show all commands

## Default Login Credentials

- **admin** / **password**
- **Narula** / **1234**
- **Krish** / **1234**
- **Shivam** / **12345678**

## Features

- SQLite database (no server required)
- User authentication
- Student management (Add, Update, Display, Delete)
- Data validation
- Portable database file

## Requirements

- C++ compiler with C++11 support
- SQLite3 library (usually pre-installed on macOS/Linux)

## Installation (if needed)

```bash
# macOS
brew install sqlite3

# Ubuntu/Debian
sudo apt install sqlite3 libsqlite3-dev
``` 