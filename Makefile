# Makefile for FRDM-KL25Z using ARM GCC and pyOCD

# Toolchain settings
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Project settings
TARGET = main
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

# Flags
CFLAGS = -mcpu=cortex-m0plus -mthumb -std=c11 -Wall -g -O2
CFLAGS += -I$(INCLUDE_DIR) -I/Users/tristan.commons/NXP/SDK_2_2_0_FRDM-KL25Z/devices/MKL25Z4
CFLAGS += -I/Users/tristan.commons/NXP/SDK_2_2_0_FRDM-KL25Z/CMSIS/Include

LDFLAGS = -T linker_script.ld -mcpu=cortex-m0plus -mthumb -nostartfiles -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Default target
all: $(BUILD_DIR)/$(TARGET).bin

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Create binary file
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(OBJCOPY) -O binary $< $@

# Upload to the board
flash: $(BUILD_DIR)/$(TARGET).bin
	pyocd flash $<

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all flash clean
