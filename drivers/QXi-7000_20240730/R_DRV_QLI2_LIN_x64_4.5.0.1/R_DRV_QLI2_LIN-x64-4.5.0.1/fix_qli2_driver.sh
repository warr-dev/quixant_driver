#!/bin/bash
# fix_qli2_driver.sh
# This script patches lin_qlimodule.c to compile on modern Linux kernels.

DRIVER_DIR="./R_DRV_QLI2_LIN-x64-4.5.0.1"  # Change this if your path is different
SOURCE_FILE="$DRIVER_DIR/lin_qlimodule.c"

if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: $SOURCE_FILE not found!"
    exit 1
fi

echo "Patching $SOURCE_FILE..."

# Backup original file
cp "$SOURCE_FILE" "$SOURCE_FILE.bak"

# Remove the old include
sed -i '/#include <asm\/unaligned.h>/d' "$SOURCE_FILE"

# Add modern replacement include and macros at the top
sed -i '1i#include <linux/unaligned/access_ok.h>\n#define get_unaligned(ptr) (*(typeof(*(ptr)) __packed *)(ptr))\n#define put_unaligned(val, ptr) (*(typeof(*(ptr)) __packed *)(ptr) = (val))\n' "$SOURCE_FILE"

echo "Patch applied. Original file backed up as lin_qlimodule.c.bak"
echo "You can now run 'make' in $DRIVER_DIR to build the driver."
