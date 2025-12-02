#!/bin/bash
# create_qli_wrapper_6.14.sh
# Creates enhanced driverscomp wrapper for QLI driver with kernel 6.14+ fixes

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "Creating QLI Kernel 6.14+ Wrapper"
echo "=========================================="
echo

# This creates a wrapper that will be integrated into qmi4l
# The wrapper patches source files for kernel 6.14+ compatibility

cat > "$SCRIPT_DIR/qli_patch_wrapper.sh" << 'WRAPPER_EOF'
#!/bin/bash
# Enhanced driverscomp wrapper for kernel 6.6+ and 6.14+ compatibility
# This script is called by qmi4l during driver installation

# Patch function for kernel 6.6+ (asm/unaligned.h issue)
patch_kernel_6_6() {
    echo "  [Kernel 6.6+ Fix] Patching header includes..."
    for cfile in *.c *.inl 2>/dev/null; do
        if [ -f "$cfile" ] && grep -q "#include <asm/unaligned.h>" "$cfile" 2>/dev/null; then
            echo "    Patching $cfile..."
            sed -i.bak 's|#include <asm/unaligned.h>|#include <linux/unaligned.h>|g' "$cfile"
        fi
    done
}

# Patch function for kernel 6.14+ (API changes for QLI driver)
patch_kernel_6_14() {
    echo "  [Kernel 6.14+ Fix] Patching API compatibility..."

    # Fix 1: vfs_mkdir API change in kqtree.inl
    if [ -f "kqtree.inl" ]; then
        echo "    Patching kqtree.inl (vfs_mkdir)..."
        # Change from &init_user_ns to &nop_mnt_idmap
        sed -i.bak 's|#define VFS_MKDIR(inode, dentry, mode) vfs_mkdir(&init_user_ns,|#define VFS_MKDIR(inode, dentry, mode) vfs_mkdir(\&nop_mnt_idmap,|g' kqtree.inl
    fi

    # Fix 2: class_create API change in qxt_device.inl
    if [ -f "qxt_device.inl" ]; then
        echo "    Patching qxt_device.inl (class_create)..."
        # Remove THIS_MODULE argument from class_create
        sed -i.bak 's|class_create( *THIS_MODULE *, *\([^)]*\) *)|class_create(\1)|g' qxt_device.inl
    fi

    # Fix 3: Add static keyword and prototypes in lin_qledmodule.c
    if [ -f "lin_qledmodule.c" ]; then
        echo "    Patching lin_qledmodule.c (prototypes & static)..."

        # Fix static keyword position
        sed -i.bak 's|const static int F\[\]|static const int F[]|g' lin_qledmodule.c

        # Make functions static to fix missing prototype warnings
        sed -i.bak2 's|^void mmap_open(|static void mmap_open(|g' lin_qledmodule.c
        sed -i.bak3 's|^void mmap_close(|static void mmap_close(|g' lin_qledmodule.c
        sed -i.bak4 's|^long qledmodule_ioctl_unlocked(|static long qledmodule_ioctl_unlocked(|g' lin_qledmodule.c
    fi
}

# Detect kernel version
KERNEL_VERSION=$(uname -r | cut -d. -f1)
KERNEL_MINOR=$(uname -r | cut -d. -f2)

echo "Detected kernel: $(uname -r)"

# Apply patches based on kernel version
if [ "$KERNEL_VERSION" -eq 6 ]; then
    if [ "$KERNEL_MINOR" -ge 14 ]; then
        echo "Kernel 6.14+ detected - applying all patches"
        patch_kernel_6_6
        patch_kernel_6_14
    elif [ "$KERNEL_MINOR" -ge 6 ]; then
        echo "Kernel 6.6-6.13 detected - applying 6.6+ patches only"
        patch_kernel_6_6
    fi
elif [ "$KERNEL_VERSION" -gt 6 ]; then
    echo "Kernel 7+ detected - applying all patches"
    patch_kernel_6_6
    patch_kernel_6_14
fi

echo "  Patching complete, ready to compile"
WRAPPER_EOF

chmod +x "$SCRIPT_DIR/qli_patch_wrapper.sh"

echo "✓ Created: qli_patch_wrapper.sh"
echo ""
echo "This wrapper will be integrated into qmi4l installer"
echo "=========================================="
