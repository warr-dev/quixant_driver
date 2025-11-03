#!/bin/bash
# create_driverscomp_wrapper.sh
# Creates a wrapper script that intercepts make calls to patch source files

set -e

DRIVERS_DIR="/home/mach-03/Pictures/QuixantModuleInstaller-QXi-7000-20240730/drivers"

echo "========================================"
echo "Creating driverscomp wrapper scripts"
echo "========================================"
echo

# Create a patched make wrapper
create_make_wrapper() {
    local dir="$1"
    local make_wrapper="$dir/make_wrapper.sh"

    cat > "$make_wrapper" << 'MAKE_WRAPPER_EOF'
#!/bin/bash
# Wrapper for make that patches source files before compilation

# First, patch any .c files that have asm/unaligned.h
for cfile in *.c; do
    if [ -f "$cfile" ] && grep -q "#include <asm/unaligned.h>" "$cfile" 2>/dev/null; then
        echo "  [Kernel 6.6+ Fix] Patching $cfile..."
        sed -i.bak 's|#include <asm/unaligned.h>|#include <linux/unaligned.h>|g' "$cfile"
    fi
done

# Now run the real make
/usr/bin/make "$@"
MAKE_WRAPPER_EOF

    chmod +x "$make_wrapper"
}

# Create driverscomp wrapper
create_driverscomp_wrapper() {
    local dir="$1"
    local comp_orig="$dir/driverscomp"
    local comp_wrapper="$dir/driverscomp_wrapper.sh"

    # Backup original if not already done
    if [ ! -f "$comp_orig.orig" ]; then
        cp "$comp_orig" "$comp_orig.orig"
    fi

    # Create make wrapper
    create_make_wrapper "$dir"

    # Create driverscomp wrapper
    cat > "$comp_wrapper" << 'COMP_WRAPPER_EOF'
#!/bin/bash
# Wrapper for driverscomp that patches kernel 6.6+ issues

# Add our make wrapper to PATH
export PATH="$(pwd):$PATH"

# Run original driverscomp, but redirect make calls to our wrapper
./driverscomp.orig "$@" 2>&1 | while IFS= read -r line; do
    echo "$line"
    # If we see the compilation error, try to patch
    if echo "$line" | grep -q "asm/unaligned.h: No such file or directory"; then
        echo "  [Kernel 6.6+ Fix] Detected asm/unaligned.h issue, patching..."
        # Patch any .c files
        for cfile in *.c; do
            if [ -f "$cfile" ] && grep -q "#include <asm/unaligned.h>" "$cfile" 2>/dev/null; then
                echo "  [Kernel 6.6+ Fix] Patching $cfile..."
                sed -i.bak 's|#include <asm/unaligned.h>|#include <linux/unaligned.h>|g' "$cfile"
            fi
        done
    fi
done

exit ${PIPESTATUS[0]}
COMP_WRAPPER_EOF

    chmod +x "$comp_wrapper"

    # Replace driverscomp with wrapper
    mv "$comp_orig" "$comp_orig.orig" 2>/dev/null || true
    cp "$comp_wrapper" "$comp_orig"
}

# Process all driver .tgz files
cd "$DRIVERS_DIR"

for tgz in R_DRV_*.tgz R_SVC_QXTCTRLHUB*.tgz; do
    if [ ! -f "$tgz" ]; then
        continue
    fi

    echo "Processing: $tgz"

    # Create temp directory
    temp_dir=$(mktemp -d)
    cd "$temp_dir"

    # Extract
    tar -xzf "$DRIVERS_DIR/$tgz" 2>/dev/null || {
        echo "  Skipping (permission denied)"
        cd "$DRIVERS_DIR"
        rm -rf "$temp_dir"
        continue
    }

    # Find extracted directory
    extracted_dir=$(find . -maxdepth 1 -type d -name "R_*" | head -1)

    if [ -z "$extracted_dir" ]; then
        echo "  No driver directory found"
        cd "$DRIVERS_DIR"
        rm -rf "$temp_dir"
        continue
    fi

    # Check if driverscomp exists
    if [ -f "$extracted_dir/driverscomp" ]; then
        echo "  Creating wrapper for driverscomp"
        create_driverscomp_wrapper "$extracted_dir"

        # Backup original .tgz
        if [ ! -f "$DRIVERS_DIR/${tgz}.orig" ]; then
            cp "$DRIVERS_DIR/$tgz" "$DRIVERS_DIR/${tgz}.orig"
        fi

        # Repackage
        tar -czf "$DRIVERS_DIR/$tgz" "$extracted_dir"
        echo "  ✓ Repackaged with wrapper"
    else
        echo "  No driverscomp found"
    fi

    # Cleanup
    cd "$DRIVERS_DIR"
    rm -rf "$temp_dir"
    echo
done

echo "========================================"
echo "Done! Run: sudo ./qmi4l -s"
echo "========================================"
