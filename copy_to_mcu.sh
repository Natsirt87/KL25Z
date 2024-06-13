#!/bin/bash

SOURCE_FILE=build/main.bin
DESTINATION_DRIVE=/media/triscuit/FRDM-KL25Z

if [ -z "$SOURCE_FILE" ] || [ -z "$DESTINATION_DRIVE" ]; then
    echo "Usage: $0 <source_file> <destination_drive>"
    exit 1
fi

cp "$SOURCE_FILE" "$DESTINATION_DRIVE/main.bin"
if [ $? -eq 0 ]; then
    echo "Binary copied successfully to $DESTINATION_DRIVE"
else
    echo "Failed to copy binary to $DESTINATION_DRIVE"
    exit 1
fi
