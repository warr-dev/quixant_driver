#!/bin/bash
# This script must be run with sudo
# It changes ownership of driver files and applies patches

if [ "$EUID" -ne 0 ]; then
    echo "Please run with sudo: sudo ./fix_permissions_and_patch.sh"
    exit 1
fi

WORK_DIR="/home/mach-03/Pictures/QuixantModuleInstaller-QXi-7000-20240730"
DRIVERS_DIR="$WORK_DIR/drivers"

echo "Changing ownership of driver files..."
chown -R mach-03:mach-03 "$DRIVERS_DIR"/*.tgz

echo "Now run: ./patch_and_repackage.sh"
