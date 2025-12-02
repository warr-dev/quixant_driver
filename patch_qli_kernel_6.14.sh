#!/bin/bash
# patch_qli_kernel_6.14.sh
# Patches QLI driver for Linux kernel 6.14+ compatibility
# Fixes:
#   1. vfs_mkdir API change: user_namespace -> mnt_idmap
#   2. class_create API change: now takes only 1 argument

set -e

echo "=========================================="
echo "QLI Driver Kernel 6.14+ Compatibility Patch"
echo "=========================================="
echo

# This script should be run from the extracted QLI driver directory
# Usage: Run this in the directory containing the .c and .inl files

# Fix 1: vfs_mkdir API change (kqtree.inl)
if [ -f "kqtree.inl" ]; then
    echo "Patching kqtree.inl for vfs_mkdir API..."

    # Check kernel version to determine which API to use
    KERNEL_VERSION=$(uname -r | cut -d. -f1)
    KERNEL_MINOR=$(uname -r | cut -d. -f2)

    if [ "$KERNEL_VERSION" -ge 6 ] && [ "$KERNEL_MINOR" -ge 14 ]; then
        # Kernel 6.14+ uses mnt_idmap
        sed -i 's|#define VFS_MKDIR(inode, dentry, mode) vfs_mkdir(&init_user_ns, (inode), (dentry), (mode))|#define VFS_MKDIR(inode, dentry, mode) vfs_mkdir(\&nop_mnt_idmap, (inode), (dentry), (mode))|g' kqtree.inl
        echo "  ✓ Updated VFS_MKDIR to use &nop_mnt_idmap"
    fi
else
    echo "  Warning: kqtree.inl not found"
fi

# Fix 2: class_create API change (qxt_device.inl)
if [ -f "qxt_device.inl" ]; then
    echo "Patching qxt_device.inl for class_create API..."

    # Replace: class_create(THIS_MODULE, class_name) with class_create(class_name)
    sed -i 's|class_create( *THIS_MODULE *, *\([^)]*\) *)|class_create(\1)|g' qxt_device.inl
    echo "  ✓ Updated class_create to single argument"
else
    echo "  Warning: qxt_device.inl not found"
fi

# Fix 3: Add missing function prototypes (lin_qledmodule.c)
if [ -f "lin_qledmodule.c" ]; then
    echo "Patching lin_qledmodule.c for missing prototypes..."

    # Check if prototypes already exist
    if ! grep -q "static void mmap_open" lin_qledmodule.c; then
        # Add function prototypes after includes (around line 100)
        # We'll add them before the first function definition

        # This is complex - we'll use a marker to insert
        # Find the line with "const static int F[]" and add static keyword fix
        sed -i 's|const static int F\[\]|static const int F[]|g' lin_qledmodule.c
        echo "  ✓ Fixed 'static' keyword position"

        # For missing prototypes, we'll add them at the top
        # Look for the last #include and add prototypes after it
        LINE_NUM=$(grep -n "#include" lin_qledmodule.c | tail -1 | cut -d: -f1)

        # Create temp file with prototypes
        {
            head -n "$LINE_NUM" lin_qledmodule.c
            echo ""
            echo "/* Function prototypes for kernel 6.14+ */"
            echo "static void mmap_open(struct vm_area_struct *vma);"
            echo "static void mmap_close(struct vm_area_struct *vma);"
            echo "static long qledmodule_ioctl_unlocked(struct file *filp, unsigned int cmd, unsigned long arg);"
            echo ""
            tail -n +"$((LINE_NUM + 1))" lin_qledmodule.c
        } > lin_qledmodule.c.tmp

        mv lin_qledmodule.c.tmp lin_qledmodule.c
        echo "  ✓ Added missing function prototypes"
    fi

    # Now make the functions static
    sed -i 's|^void mmap_open|static void mmap_open|g' lin_qledmodule.c
    sed -i 's|^void mmap_close|static void mmap_close|g' lin_qledmodule.c
    sed -i 's|^long qledmodule_ioctl_unlocked|static long qledmodule_ioctl_unlocked|g' lin_qledmodule.c
    echo "  ✓ Made functions static"
else
    echo "  Warning: lin_qledmodule.c not found"
fi

echo ""
echo "=========================================="
echo "Patching complete!"
echo "Now run: make"
echo "=========================================="
