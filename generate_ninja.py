import os

src_dir = "src"
platform_dir = "platform"
build_dir = "build"
platform_mkl25z4_dir = "platform/MKL25Z4/startup/gcc"
include_dir = "include"
output_file = "build.ninja"

cflags = (
    "-Wall -Icore -mcpu=cortex-m0plus --specs=nosys.specs -mthumb "
    "-ffunction-sections -fdata-sections -fmessage-length=0 -mfloat-abi=soft "
    "-Iplatform/MKL25Z4/include -Iinclude -DCPU_MKL25Z128VFM4=1"
)
libs = "-I./deps"

rules = """
rule cc
  command = arm-none-eabi-gcc $cflags $libs -c $in -o $out

rule pack
  command = arm-none-eabi-gcc-ar -r $out $in

rule link
  command = arm-none-eabi-gcc $cflags -T platform/MKL25Z4/linker/gcc/MKL25Z128xxx4_flash.ld -o $out $in -L ./build -l_MKL25Z4

rule tobinary
  command = arm-none-eabi-objcopy -O binary $in $out

rule flash
  command = powershell.exe -ExecutionPolicy Bypass -File copy_to_mcu.ps1 -SourceFile $in -DestinationDrive E:
  description = Copying binary to MCU

rule clean
  command = rm -rf build/*
  description = Cleaning build files
"""

def generate_ninja_file():
    with open(output_file, "w") as f:
        f.write(f"cflags = {cflags}\n")
        f.write(f"libs = {libs}\n")
        f.write(rules)

        # Get all header files in the include directory
        header_files = [
            os.path.join(include_dir, header_file)
            for header_file in os.listdir(include_dir)
            if header_file.endswith(".h")
        ]
        header_files_str = " ".join(header_files)

        # Compile src/*.c files
        for src_file in os.listdir(src_dir):
            if src_file.endswith(".c"):
                obj_file = os.path.join(build_dir, f"{os.path.splitext(src_file)[0]}.o")
                src_file_path = os.path.join(src_dir, src_file)
                f.write(f"build {obj_file} : cc {src_file_path} | {header_files_str}\n")

        # Compile platform/*.c files
        platform_files = ["system_MKL25Z4.c", "startup.c"]
        for platform_file in platform_files:
            obj_file = os.path.join(build_dir, f"{os.path.splitext(platform_file)[0]}.o")
            platform_file_path = os.path.join(platform_dir, platform_file)
            f.write(f"build {obj_file} : cc {platform_file_path}\n")

        # Compile platform/MKL25Z4/startup/gcc/*.S files
        startup_files = ["startup_MKL25Z4.S"]
        for startup_file in startup_files:
            obj_file = os.path.join(build_dir, f"{os.path.splitext(startup_file)[0]}.o")
            startup_file_path = os.path.join(platform_mkl25z4_dir, startup_file)
            f.write(f"build {obj_file} : cc {startup_file_path}\n")

        # Create startup library
        f.write(f"build {build_dir}/lib_MKL25Z4.a : pack {build_dir}/startup.o {build_dir}/system_MKL25Z4.o {build_dir}/startup_MKL25Z4.o\n")

        # List all object files for linking
        obj_files = [
            f"{build_dir}/{os.path.splitext(file)[0]}.o"
            for file in os.listdir(src_dir)
            if file.endswith(".c")
        ]
        obj_files.extend([
            f"{build_dir}/startup.o",
            f"{build_dir}/system_MKL25Z4.o",
            f"{build_dir}/startup_MKL25Z4.o"
        ])

        # Link all in a .elf file
        obj_files_str = " ".join(obj_files)
        f.write(f"build {build_dir}/main.elf : link {obj_files_str} {build_dir}/lib_MKL25Z4.a\n")

        # Convert the .elf into a raw binary file that can be drag'n'drop on the board (with OpenSDA mass-storage bootloader)
        f.write(f"build {build_dir}/main.bin : tobinary {build_dir}/main.elf\n")

        # Flash
        f.write(f"build flash : flash {build_dir}/main.bin\n")

        # Clean
        f.write("build clean: clean\n")

        # Rebuild (clean and then build all)
        f.write("build rebuild: phony clean build/main.bin\n")

        # Default target
        f.write("default build/main.bin\n")

if __name__ == "__main__":
    generate_ninja_file()
    print(f"Ninja build file generated: {output_file}")
