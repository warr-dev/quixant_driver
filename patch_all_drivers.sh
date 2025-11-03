#!/bin/bash
# patch_all_drivers.sh
# Patches all Quixant drivers for Linux kernel 6.6+ compatibility
# The issue: asm/unaligned.h was removed in kernel 6.6+
# Solution: Replace with linux/unaligned.h (available in kernel 6.6+)

set -e

DRIVERS_BASE="/home/mach-03/Pictures/QuixantModuleInstaller-QXi-7000-20240730/drivers/QXi-7000_20240730"

echo "=================================="
echo "Quixant Driver Kernel 6.6+ Patcher"
echo "=================================="
echo

# Function to patch driverscomp script
patch_driverscomp() {
    local driver_dir="$1"
    local comp_script="$driver_dir/driverscomp"

    if [ ! -f "$comp_script" ]; then
        echo "  Warning: driverscomp not found in $driver_dir"
        return 1
    fi

    # Check if it's a binary (can't patch) or script
    if file "$comp_script" | grep -q "ELF"; then
        echo "  driverscomp is a binary, creating wrapper..."

        # Rename original
        if [ ! -f "$comp_script.orig" ]; then
            mv "$comp_script" "$comp_script.orig"
        fi

        # Create wrapper script
        cat > "$comp_script" << 'WRAPPER_EOF'
#!/bin/bash
# Wrapper for driverscomp that patches source files for kernel 6.6+

# Run original driverscomp to extract sources
./driverscomp.orig "$@"
EXIT_CODE=$?

# If extraction failed, exit
if [ $EXIT_CODE -ne 0 ]; then
    # Try to patch and recompile
    echo "Compilation failed, attempting to patch for kernel 6.6+..."

    # Find all .c files that include asm/unaligned.h
    for cfile in *.c; do
        if [ -f "$cfile" ] && grep -q "#include <asm/unaligned.h>" "$cfile" 2>/dev/null; then
            echo "  Patching $cfile..."

            # Create backup
            cp "$cfile" "$cfile.bak"

            # Replace asm/unaligned.h with linux/unaligned.h
            sed -i 's|#include <asm/unaligned.h>|#include <linux/unaligned.h>|g' "$cfile"
        fi
    done

    # Try to compile again
    if [ -f "Makefile" ]; then
        echo "  Recompiling with patched sources..."
        make clean >/dev/null 2>&1 || true
        make
        EXIT_CODE=$?
    fi
fi

exit $EXIT_CODE
WRAPPER_EOF

        chmod +x "$comp_script"
        echo "  ✓ Wrapper created for $driver_dir"
        return 0
    fi
}

# Patch all driver directories
echo "Patching driver compilation scripts..."
echo

for driver in "$DRIVERS_BASE"/R_DRV_*/ "$DRIVERS_BASE"/R_SVC_QXTCTRLHUB_*/; do
    if [ -d "$driver" ]; then
        driver_name=$(basename "$driver")
        echo "Processing: $driver_name"

        # Find the actual driver subdirectory containing driverscomp
        inner_dir=$(find "$driver" -type f -name "driverscomp" -o -name "driverscomp.orig" | head -1 | xargs dirname)

        if [ -n "$inner_dir" ] && [ -d "$inner_dir" ]; then
            patch_driverscomp "$inner_dir"
        else
            echo "  Warning: driverscomp not found in any subdirectory"
        fi
        echo
    fi
done

echo "=================================="
echo "Patching complete!"
echo "You can now run: sudo ./qmi4l -s"
echo "=================================="
