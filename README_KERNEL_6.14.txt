================================================================================
KERNEL 6.14 COMPATIBILITY - START HERE
================================================================================

Welcome! This document helps you navigate the kernel 6.14+ compatibility
documentation for the Quixant driver installer.

================================================================================
QUICK START
================================================================================

Just want to install? Run this:

    sudo ./qmi4l -s

That's it! The installer automatically handles all kernel 6.14 compatibility.

================================================================================
DOCUMENTATION INDEX
================================================================================

📋 START HERE (Pick One):
┌─────────────────────────────────────────────────────────────────────────┐
│ README_KERNEL_6.14.txt         This file - Documentation index          │
│ KERNEL_6.14_SUMMARY.txt        Executive summary of changes             │
│ KERNEL_6.14_QUICK_REFERENCE.txt Quick reference card                    │
└─────────────────────────────────────────────────────────────────────────┘

📖 INSTALLATION & USAGE:
┌─────────────────────────────────────────────────────────────────────────┐
│ INSTALLATION_READY.txt         Complete installation guide              │
│ TESTING_GUIDE.md               How to test drivers after installation   │
└─────────────────────────────────────────────────────────────────────────┘

🔧 TECHNICAL DOCUMENTATION:
┌─────────────────────────────────────────────────────────────────────────┐
│ KERNEL_6.14_FIX_README.md      Comprehensive technical documentation    │
│ CHANGELOG.md                   Version history and detailed changes     │
│ COMPLETE_DOCUMENTATION.md      Full project documentation               │
└─────────────────────────────────────────────────────────────────────────┘

🛠️ UTILITY SCRIPTS:
┌─────────────────────────────────────────────────────────────────────────┐
│ patch_qli_kernel_6.14.sh       Standalone patching script               │
│ qli_patch_wrapper.sh           Modular wrapper with patch logic         │
│ create_qli_wrapper_6.14.sh     Wrapper generation script                │
└─────────────────────────────────────────────────────────────────────────┘

================================================================================
CHOOSE YOUR PATH
================================================================================

I just want to install drivers:
→ Run: sudo ./qmi4l -s
→ Read: INSTALLATION_READY.txt (if you have questions)

I want a quick overview:
→ Read: KERNEL_6.14_SUMMARY.txt
→ Then: KERNEL_6.14_QUICK_REFERENCE.txt

I need technical details:
→ Read: KERNEL_6.14_FIX_README.md
→ Reference: CHANGELOG.md

I'm having issues:
→ Check: KERNEL_6.14_QUICK_REFERENCE.txt (Troubleshooting section)
→ Read: KERNEL_6.14_FIX_README.md (Troubleshooting section)

I want to understand what changed:
→ Read: CHANGELOG.md
→ Review: git log (if you want commit details)

================================================================================
WHAT'S IN EACH DOCUMENT
================================================================================

README_KERNEL_6.14.txt (This File)
----------------------------------
• Documentation index
• Quick navigation guide
• What to read based on your needs

KERNEL_6.14_SUMMARY.txt
-----------------------
• High-level overview
• Problem and solution
• Testing results
• Success indicators
Best for: Quick understanding of what was done

KERNEL_6.14_QUICK_REFERENCE.txt
--------------------------------
• Command reference
• Error fixes at a glance
• Troubleshooting tips
• API changes cheat sheet
Best for: Quick lookup while working

KERNEL_6.14_FIX_README.md
-------------------------
• Detailed technical documentation
• Complete API change explanations
• In-depth troubleshooting
• Kernel API reference
• Implementation details
Best for: Deep understanding and debugging

CHANGELOG.md
------------
• Version history
• Detailed change log
• Migration guide
• What changed and why
Best for: Understanding evolution of the project

INSTALLATION_READY.txt
----------------------
• Installation instructions
• System requirements
• Hardware detection info
• Common commands
Best for: Installing and using the system

COMPLETE_DOCUMENTATION.md
-------------------------
• Full project documentation
• All features and capabilities
• Test programs
• API usage examples
Best for: Complete project understanding

================================================================================
WHAT WAS FIXED
================================================================================

The QLI driver failed to compile on kernel 6.14 due to API changes.

Fixed Issues:
✓ vfs_mkdir API incompatibility
✓ class_create API incompatibility
✓ Static keyword position warnings
✓ Missing function prototype warnings

Solution:
✓ Automatic kernel detection
✓ Dynamic patching during compilation
✓ Zero user intervention required
✓ Full backward compatibility

Result:
✓ All drivers compile successfully on kernel 6.14+
✓ Automatic installation works perfectly
✓ Comprehensive documentation provided

================================================================================
FILE ORGANIZATION
================================================================================

Documentation Files:
├── README_KERNEL_6.14.txt              ← You are here
├── KERNEL_6.14_SUMMARY.txt             Summary
├── KERNEL_6.14_QUICK_REFERENCE.txt     Quick reference
├── KERNEL_6.14_FIX_README.md           Technical docs
├── CHANGELOG.md                        Change history
├── INSTALLATION_READY.txt              Install guide
├── COMPLETE_DOCUMENTATION.md           Full docs
└── TESTING_GUIDE.md                    Testing guide

Core Files:
├── qmi4l                               Enhanced installer (v2.0.2.3-k6.14)
└── Makefile                            Build file for test programs

Utility Scripts:
├── patch_qli_kernel_6.14.sh           Standalone patcher
├── qli_patch_wrapper.sh               Modular wrapper
└── create_qli_wrapper_6.14.sh         Wrapper generator

Test Programs:
├── test_qxtio.c                       Basic device test
├── core_io_example.c                  CORE I/O example
└── button_monitor.c                   Button monitoring

Driver Archives:
└── drivers/
    ├── R_DRV_CORE_LIN_x64_0.7.0.1.tgz
    ├── R_DRV_NVRAM_LIN_x64_3.9.2.5.tgz
    ├── R_DRV_TRACER_LIN_x64_2.3.1.0.tgz
    ├── R_DRV_QLI_LIN_x64_3.1.1.0.tgz        ← Needs 6.14 patches
    ├── R_DRV_QXTSECS_LIN_x64_1.6.0.1.tgz
    └── R_DRV_QLI2_LIN_x64_4.5.0.1.tgz

================================================================================
COMMON TASKS
================================================================================

Install drivers:
    sudo ./qmi4l -s

Check installation status:
    sudo ./qmi4l -p

Uninstall drivers:
    sudo ./qmi4l -u

View installed version:
    ./qmi4l -h | grep VERSION

Test basic functionality:
    make test

Test CORE I/O:
    make demo

Check loaded modules:
    lsmod | grep qxt

View Quixant hardware:
    lspci -nn | grep 19d4

Check kernel version:
    uname -r

View device nodes:
    ls -l /dev/qxt*

================================================================================
GETTING HELP
================================================================================

Step 1: Check Quick Reference
    → KERNEL_6.14_QUICK_REFERENCE.txt

Step 2: Read Relevant Documentation
    → Installation issues: INSTALLATION_READY.txt
    → Technical issues: KERNEL_6.14_FIX_README.md
    → Understanding changes: CHANGELOG.md

Step 3: Review Error Messages
    → Check Troubleshooting sections in docs
    → Compare with known issues

Step 4: Verify Setup
    → Kernel version: uname -r
    → Installer version: ./qmi4l -h | grep VERSION
    → PCI devices: lspci -nn | grep 19d4

================================================================================
SYSTEM REQUIREMENTS
================================================================================

Supported:
✓ Ubuntu 24.04 LTS
✓ Linux kernel 6.14.0-36-generic (and newer)
✓ gcc-13 compiler
✓ Quixant QXi-7000 hardware

Tested:
✓ Kernel 6.14.0-36-generic
✓ All Quixant drivers compile successfully
✓ Backward compatible with kernel 6.6+

================================================================================
SUCCESS CHECKLIST
================================================================================

After installation, verify:

□ No "ERROR compiling" messages during installation
□ Kernel modules created (*.ko files)
□ Modules load without errors: lsmod | grep qxt
□ Device nodes exist: ls /dev/qxt*
□ Test programs work: ./test_qxtio
□ Your application can access devices

Ignore these warnings:
✓ "warning: the compiler differs..."
✓ "Skipping BTF generation..."

================================================================================
NEED MORE INFORMATION?
================================================================================

Technical Details:        KERNEL_6.14_FIX_README.md
Quick Reference:          KERNEL_6.14_QUICK_REFERENCE.txt
Installation:             INSTALLATION_READY.txt
Complete Documentation:   COMPLETE_DOCUMENTATION.md
Version History:          CHANGELOG.md

================================================================================
VERSION INFO
================================================================================

Installer Version: 2.0.2.3-k6.14
Date: 2025-12-02
Kernel Support: 6.6+ (full support for 6.14+)
Status: Production ready

Previous Version: 2.0.2.3-k6.6 (kernel 6.6-6.13 support)
Original Version: 2.0.2.3 (no kernel 6.6+ support)

================================================================================
READY TO PROCEED
================================================================================

The Quixant driver installer is ready for use on Linux kernel 6.14+

Run: sudo ./qmi4l -s

For questions, consult the documentation files listed above.

================================================================================
