# Compiler and project settings.
CC       ?= gcc
TARGET   := ash

# Directory layout for source files, headers, and generated build artifacts.
SRC_DIR   := src
INC_DIR   := headers
BUILD_DIR := build

# Compiler flags:
# -Wall/-Wextra enable useful diagnostics.
# -Iheaders lets source files include project headers by name.
# -MMD/-MP create dependency files so header changes trigger rebuilds.
CFLAGS   ?= -Wall -Wextra -I$(INC_DIR) -MMD -MP
LDFLAGS  ?=
LDLIBS   ?=

# Gather every C source file and map it to a matching object file in build/.
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Default target: build the shell executable.
all: $(TARGET)

# Link all compiled object files into the final program.
$(TARGET): $(OBJS)
	@$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compile each source file into an object file.
# The order-only prerequisite creates build/ without forcing recompilation.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory when it does not already exist.
$(BUILD_DIR):
	@mkdir -p $@

# Remove generated files and the executable.
clean:
	@rm -rf $(BUILD_DIR) $(TARGET)

# Rebuild everything from scratch.
re: clean all

# Include generated header dependency files when they exist.
-include $(DEPS)

# These names are Make targets, not real files.
.PHONY: all clean re
