# Compiler and Flags
CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -Iheaders
TARGET   := ash

# Directories
SRC_DIR  := src
OBJ_DIR  := obj

# Files
SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default rule
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Build successful!"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Clean complete."

# Phony targets
.PHONY: all clean