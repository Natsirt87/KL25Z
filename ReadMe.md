# Bare-Metal KL25Z Project

This is an educational and exploratory project for embedded systems learning in which I am doing bare-metal development with CMSIS on a FRDM-KL25Z board from NXP. I'm keeping a reference guide for how to perform various tasks, as well as some techniques and best practices

## Running the project

The build system is custom developed, and integrates the GNU ARM GCC toolchain with the NXP SDK and a ninja build file generator.

To build the project, first make sure you have the latest versions of `arm-none-eabi-gcc` and `ninja` installed.
Then run `python3 generate_ninja.py` to generate the build script. Run `ninja` to build, and run `ninja flash` to copy the binary to the E drive, assuming that's where the KL25Z board is mounted. If it's mounted somewhere else, change the powershell script `copy_to_mcu.ps1` to reflect that.

If on Linux, simply run the `copy_to_mcu.sh` bash script, making sure to update the destination to wherever your KL25Z is mounted on your system.