# Compiler and flags
CC = arm-none-eabi-gcc
AR = arm-none-eabi-gcc-ar
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -Wall -Icore -mcpu=cortex-m0plus --specs=nosys.specs -mthumb -ffunction-sections -fdata-sections -fmessage-length=0 -mfloat-abi=soft
LIBS = -I./deps
LDFLAGS = -T platform/MKL25Z4/linker/gcc/MKL25Z128xxx4_flash.ld -L ./build -l_MKL25Z4

# Directories
SRC_DIR = src
PLATFORM_DIR = platform
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(PLATFORM_DIR)/*.c) \
       $(wildcard $(PLATFORM_DIR)/MKL25Z4/startup/gcc/*.S)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter %.c,$(SRCS))) \
       $(patsubst $(PLATFORM_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter %.c,$(SRCS))) \
       $(patsubst $(PLATFORM_DIR)/MKL25Z4/startup/gcc/%.S, $(BUILD_DIR)/%.o, $(filter %.S,$(SRCS)))

# Targets
TARGET_ELF = $(BUILD_DIR)/main.elf
TARGET_BIN = $(BUILD_DIR)/main.bin
LIB_MKL25Z4 = $(BUILD_DIR)/lib_MKL25Z4.a

# Default target
all: $(TARGET_BIN)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(LIBS) -DCPU_MKL25Z128VFM4=1 -c $< -o $@

$(BUILD_DIR)/%.o: $(PLATFORM_DIR)/%.c
	$(CC) $(CFLAGS) $(LIBS) -DCPU_MKL25Z128VFM4=1 -c $< -o $@

$(BUILD_DIR)/%.o: $(PLATFORM_DIR)/MKL25Z4/startup/gcc/%.S
	$(CC) $(CFLAGS) $(LIBS) -DCPU_MKL25Z128VFM4=1 -c $< -o $@

# Create startup library
$(LIB_MKL25Z4): $(BUILD_DIR)/startup.o $(BUILD_DIR)/system_MKL25Z4.o $(BUILD_DIR)/startup_MKL25Z4.o
	$(AR) -r $@ $^

# Link all object files into a .elf file
$(TARGET_ELF): $(OBJS) $(LIB_MKL25Z4)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIB_MKL25Z4)

# Convert the .elf into a raw binary file
$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

# Flash the binary
flash: $(TARGET_BIN)
	powershell.exe -ExecutionPolicy Bypass -File copy_to_e.ps1 -SourceFile $< -DestinationDrive E:

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*

# Rebuild (clean and then build all)
rebuild: clean all

.PHONY: all clean flash rebuild
