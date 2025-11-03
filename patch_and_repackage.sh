#!/bin/bash
# patch_and_repackage.sh
# Extracts driver .tgz files, patches them for kernel 6.6+, and repackages them

set -e

WORK_DIR="/home/mach-03/Pictures/QuixantModuleInstaller-QXi-7000-20240730"
DRIVERS_DIR="$WORK_DIR/drivers"
TEMP_DIR="$WORK_DIR/temp_patch"

echo "=========================================="
echo "Quixant Driver Archive Patcher"
echo "=========================================="
echo

# Create temp directory
rm -rf "$TEMP_DIR"
mkdir -p "$TEMP_DIR"

# Function to create patched driverscomp wrapper
create_wrapper() {
    local target_dir="$1"
    local comp_script="$target_dir/driverscomp"

    if [ ! -f "$comp_script" ]; then
        return 1
    fi

    # Check if it's a binary
    if file "$comp_script" | grep -q "ELF"; then
        echo "    Creating patched wrapper..."

        # Backup original if not already done
        if [ ! -f "$comp_script.orig" ]; then
            cp "$comp_script" "$comp_script.orig"
        fi

        # Create wrapper script
        cat > "$comp_script" << 'WRAPPER_EOF'
#!/bin/bash
# Wrapper for driverscomp that patches source files for kernel 6.6+

# Run original driverscomp to extract sources
./driverscomp.orig "$@"
EXIT_CODE=$?

# If extraction failed, try to patch and recompile
if [ $EXIT_CODE -ne 0 ]; then
    echo "Compilation failed, attempting to patch for kernel 6.6+..."

    # Find all .c files that include asm/unaligned.h
    PATCHED=0
    for cfile in *.c; do
        if [ -f "$cfile" ] && grep -q "#include <asm/unaligned.h>" "$cfile" 2>/dev/null; then
            echo "  Patching $cfile..."

            # Create backup
            cp "$cfile" "$cfile.bak"

            # Replace asm/unaligned.h with linux/unaligned.h
            sed -i 's|#include <asm/unaligned.h>|#include <linux/unaligned.h>|g' "$cfile"
            PATCHED=1
        fi
    done

    # Try to compile again if we patched something
    if [ $PATCHED -eq 1 ] && [ -f "Makefile" ]; then
        echo "  Recompiling with patched sources..."
        make clean >/dev/null 2>&1 || true
        make
        EXIT_CODE=$?
    fi
fi

exit $EXIT_CODE
WRAPPER_EOF

        chmod +x "$comp_script"
        return 0
    fi
    return 1
}

# Process each driver .tgz file
cd "$DRIVERS_DIR"

for tgz in R_DRV_*.tgz R_SVC_QXTCTRLHUB*.tgz; do
    if [ ! -f "$tgz" ]; then
        continue
    fi

    echo "Processing: $tgz"

    # Extract to temp directory
    cd "$TEMP_DIR"
    tar -xzf "$DRIVERS_DIR/$tgz"

    # Find the extracted directory
    extracted_dir=$(ls -d R_* 2>/dev/null | head -1)

    if [ -z "$extracted_dir" ]; then
        echo "  ERROR: Could not find extracted directory"
        continue
    fi

    echo "  Extracted: $extracted_dir"

    # Create wrapper for driverscomp
    if create_wrapper "$extracted_dir"; then
        echo "  ✓ Patched wrapper created"

        # Backup original .tgz
        if [ ! -f "$DRIVERS_DIR/${tgz}.orig" ]; then
            cp "$DRIVERS_DIR/$tgz" "$DRIVERS_DIR/${tgz}.orig"
            echo "  ✓ Original backed up as ${tgz}.orig"
        fi

        # Repackage
        tar -czf "$DRIVERS_DIR/$tgz" "$extracted_dir"
        echo "  ✓ Repackaged: $tgz"
    else
        echo "  ⊘ No driverscomp found, skipping"
    fi

    # Clean up temp directory
    rm -rf "$TEMP_DIR"/*
    echo

done

# Clean up
rm -rf "$TEMP_DIR"

echo "=========================================="
echo "Patching complete!"
echo "Original .tgz files backed up as .orig"
echo "You can now run: sudo ./qmi4l -s"
echo "=========================================="
