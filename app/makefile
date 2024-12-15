# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Iinclude/server -Iinclude/view

# Directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
INCLUDE_DIR = include

# Subdirectories
VIEW_DIR = $(SRC_DIR)/view
SERVER_DIR = $(SRC_DIR)/server
CMD_DIR = $(SERVER_DIR)/cmd

# Source files
VIEW_SRCS = $(wildcard $(VIEW_DIR)/*.c)
SERVER_SRCS = $(wildcard $(SERVER_DIR)/*.c) $(wildcard $(CMD_DIR)/*.c)
IPC_SRCS = $(SRC_DIR)/ipc.c

# Object files
VIEW_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(VIEW_SRCS) $(IPC_SRCS))
SERVER_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SERVER_SRCS) $(IPC_SRCS))

# Targets
VIEW_TARGET = view
SERVER_TARGET = server

# Default target
all: check_dependencies $(VIEW_TARGET) $(SERVER_TARGET)

# Check if GTK is installed, and install if missing
check_dependencies:
	@if ! pkg-config --exists gtk+-3.0; then \
		echo "GTK+-3.0 not found. Installing..."; \
		sudo apt-get update && sudo apt-get install -y libgtk-3-dev; \
	else \
		echo "GTK+-3.0 is installed."; \
	fi

# Create build and object directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/view
	mkdir -p $(OBJ_DIR)/server
	mkdir -p $(OBJ_DIR)/server/cmd

# Build rules for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< `pkg-config --cflags gtk+-3.0` -o $@

# Build rules for executables
$(VIEW_TARGET): $(VIEW_OBJS)
	$(CC) $(CFLAGS) $^ `pkg-config --cflags --libs gtk+-3.0` -o $@

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
