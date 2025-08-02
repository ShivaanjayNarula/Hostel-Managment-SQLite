# Makefile for Hostel Management System with SQLite

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# SQLite library flags
SQLITE_LIBS = -lsqlite3

# Target executable
TARGET = hostel_sqlite

# Source files
SOURCES = main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Compile the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SQLITE_LIBS)

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET) hostel_management.db

# Install dependencies (macOS)
install-deps:
	brew install sqlite3

# Run the application
run: $(TARGET)
	./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the application"
	@echo "  clean        - Remove build files and database"
	@echo "  install-deps - Install SQLite (macOS)"
	@echo "  run          - Build and run the application"
	@echo "  help         - Show this help message"

.PHONY: all clean install-deps run help 