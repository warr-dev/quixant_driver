# Quixant Module Installer for Linux (QMI4L) - Kernel 6.14+ Compatible

Enhanced version of the Quixant Module Installer with full support for Linux kernel 6.6+ and 6.14+.

[![Version](https://img.shields.io/badge/version-2.0.2.3--k6.14-blue)](./INSTALLATION_READY.txt)
[![Kernel](https://img.shields.io/badge/kernel-6.14%2B-green)](./docs/KERNEL_6.14_FIX_README.md)
[![License](https://img.shields.io/badge/license-Quixant-orange)](.)

---

## Quick Start

```bash
sudo ./qmi4l -s
```

That's it! The installer automatically handles all kernel 6.6+ and 6.14+ compatibility issues.

---

## What's New in v2.0.2.3-k6.14

✅ **Full kernel 6.14+ support** - Automatically patches QLI driver for new kernel APIs
✅ **Zero user intervention** - All patches applied during compilation
✅ **Backward compatible** - Works on kernel 6.6+ and older
✅ **Comprehensive documentation** - Detailed guides and troubleshooting

### Kernel Compatibility

| Kernel Version | Status | Notes |
|----------------|--------|-------|
| < 6.6 | ✅ Native | No patches needed |
| 6.6 - 6.13 | ✅ Supported | Header fix only |
| 6.14+ | ✅ Fully Supported | All API patches applied automatically |
| 7.0+ | ✅ Future-proof | Ready for next major kernel |

---

## Documentation

### 📖 Essential Reading

- **[README_KERNEL_6.14.txt](README_KERNEL_6.14.txt)** - Start here! Documentation index
- **[INSTALLATION_READY.txt](INSTALLATION_READY.txt)** - Installation guide
- **[docs/KERNEL_6.14_QUICK_REFERENCE.txt](docs/KERNEL_6.14_QUICK_REFERENCE.txt)** - Quick reference card

### 📚 Detailed Documentation

- **[docs/KERNEL_6.14_FIX_README.md](docs/KERNEL_6.14_FIX_README.md)** - Complete technical documentation
- **[docs/CHANGELOG.md](docs/CHANGELOG.md)** - Version history and changes
- **[docs/COMPLETE_DOCUMENTATION.md](docs/COMPLETE_DOCUMENTATION.md)** - Full project documentation

---

## Supported Systems

### Operating Systems
- Ubuntu 18.04 LTS
- Ubuntu 20.04 LTS
- Ubuntu 22.04 LTS
- Ubuntu 24.04 LTS ✨ **Recommended**

### Hardware
- QXi-7000 series
- QXc-70 series
- IQ Connect 1

### Tested On
- **OS:** Ubuntu 24.04 LTS
- **Kernel:** 6.14.0-36-generic
- **Compiler:** gcc-13

---

## Installation

### Prerequisites

```bash
# Install kernel headers
sudo apt install linux-headers-$(uname -r)

# Install required packages
sudo apt install build-essential libusb-1.0-0-dev
```

### Install Drivers

```bash
# Extract the archive (if not already done)
tar -xzvf R_TOOLS_QMI4L_*.tgz
cd QuixantModuleInstaller-*

# Install drivers and services
sudo ./qmi4l -s
```

### Verify Installation

```bash
# Check driver status
sudo ./qmi4l -p

# List loaded modules
lsmod | grep qxt

# Test basic functionality
make test
```

---

## Usage

### Commands

```bash
sudo ./qmi4l -s    # Install drivers and services
sudo ./qmi4l -u    # Uninstall drivers and services
sudo ./qmi4l -p    # Show status of loaded modules
sudo ./qmi4l -h    # Display help
```

### Test Programs

```bash
make              # Build all test programs
make test         # Build and run basic test
make demo         # Build and run CORE I/O example
make clean        # Remove compiled binaries
```

---

## What Was Fixed

### Kernel 6.6+ Issues

**Problem:** Header file removed in kernel 6.6
**Error:** `fatal error: asm/unaligned.h: No such file or directory`
**Fix:** Automatically replaced with `linux/unaligned.h`

### Kernel 6.14+ Issues (QLI Driver)

1. **VFS API Change**
   - **Error:** `passing argument 1 of 'vfs_mkdir' from incompatible pointer type`
   - **Fix:** `&init_user_ns` → `&nop_mnt_idmap`

2. **Class API Change**
   - **Error:** `too many arguments to function 'class_create'`
   - **Fix:** `class_create(THIS_MODULE, name)` → `class_create(name)`

3. **Code Style Warnings**
   - **Error:** `'static' is not at beginning of declaration`
   - **Fix:** `const static` → `static const`

All fixes are applied **automatically** during driver compilation!

---

## Repository Structure

```
.
├── README.md                      # This file
├── README_KERNEL_6.14.txt         # Kernel 6.14 documentation index
├── INSTALLATION_READY.txt         # Installation guide
├── qmi4l                          # Main installer (v2.0.2.3-k6.14)
├── Makefile                       # Build system for test programs
│
├── docs/                          # Documentation
│   ├── KERNEL_6.14_FIX_README.md      # Technical documentation
│   ├── KERNEL_6.14_QUICK_REFERENCE.txt # Quick reference
│   ├── KERNEL_6.14_SUMMARY.txt        # Summary of changes
│   ├── CHANGELOG.md                   # Version history
│   ├── COMPLETE_DOCUMENTATION.md      # Full project docs
│   └── TESTING_GUIDE.md               # Testing guide
│
├── examples/                      # Example code
│   ├── test_qxtio.c                   # Basic device test
│   ├── core_io_example.c              # CORE I/O example
│   ├── button_monitor.c               # Button monitoring
│   └── io_quixant.cpp/.h              # C++ interface
│
├── scripts/                       # Utility scripts
│   ├── patch_all_drivers.sh           # Batch driver patcher
│   └── test_quixant.sh                # Test script
│
└── drivers/                       # Driver packages
    ├── R_DRV_CORE_LIN_x64_0.7.0.1.tgz
    ├── R_DRV_NVRAM_LIN_x64_3.9.2.5.tgz
    ├── R_DRV_QLI_LIN_x64_3.1.1.0.tgz      # Requires 6.14 patches
    └── ...
```

---

## Troubleshooting

### Compilation Fails

1. **Check kernel version:** `uname -r`
2. **Verify installer version:** `./qmi4l -h | grep VERSION` (should show 2.0.2.3-k6.14)
3. **Install kernel headers:** `sudo apt install linux-headers-$(uname -r)`
4. **Check documentation:** [docs/KERNEL_6.14_QUICK_REFERENCE.txt](docs/KERNEL_6.14_QUICK_REFERENCE.txt)

### Drivers Don't Load

```bash
# Check for errors
dmesg | grep -i qxt

# Check module info
modinfo /lib/modules/$(uname -r)/updates/dkms/*.ko

# Verify PCI devices
lspci -nn | grep 19d4
```

### Safe to Ignore

✅ `warning: the compiler differs from the one used to build the kernel` - False positive
✅ `Skipping BTF generation...` - BTF is optional (only for eBPF debugging)

---

## Development

### Building Test Programs

```bash
# Build all
make

# Build specific program
make test_qxtio
make core_io_example

# Run tests
make test
make demo
```

### Example Code

See the [examples/](examples/) directory for:
- Basic I/O operations
- Button monitoring
- CORE device access
- C++ interface

---

## Operations Performed by QMI4L

- ✅ Driver installation and compilation (with kernel 6.14+ patches)
- ✅ System service setup (qxtDrv, rc-local)
- ✅ Serial port renaming
- ✅ USB device udev rules
- ✅ User group configuration (dialout, plugdev)
- ✅ Automatic kernel version detection

---

## Technical Details

### How Patching Works

1. User runs `sudo ./qmi4l -s`
2. Installer extracts driver archives
3. Detects kernel version automatically
4. Creates wrapper for `driverscomp` binary
5. Wrapper patches source files before compilation:
   - Kernel 6.6+: Header fix
   - Kernel 6.14+: API compatibility patches
6. Compilation succeeds, installation proceeds

### Patch Locations

- **qmi4l:** Lines 596-649 (DRV section), Lines 727-780 (CTRLHUB section)
- **Auto-applied during:** Driver compilation
- **No manual steps:** Everything is automatic

---

## Version History

### 2.0.2.3-k6.14 (2025-12-02) - This Version
+ [enhancement] Added full kernel 6.14+ support
+ [enhancement] Automatic VFS API patching (vfs_mkdir)
+ [enhancement] Automatic class API patching (class_create)
+ [enhancement] Code style fixes (static keyword, prototypes)
+ [enhancement] Comprehensive documentation
+ [enhancement] Backward compatible with kernel 6.6+

### 2.0.2.3-k6.6 (2025-11-03)
+ [enhancement] Added kernel 6.6+ support
+ [bugfix] Fixed asm/unaligned.h header issue

### 2.0.2.3 (2024-12-12) - Original Quixant Version
+ [bugfix] Serial port scan using udevadm (Mantis #632)
+ [bugfix] Fixed unexpected printouts (Mantis #656)
+ [improvement] Added support for "Iq Connect 1"

[See full changelog →](docs/CHANGELOG.md)

---

## Support

### Documentation
- Quick help: [docs/KERNEL_6.14_QUICK_REFERENCE.txt](docs/KERNEL_6.14_QUICK_REFERENCE.txt)
- Technical details: [docs/KERNEL_6.14_FIX_README.md](docs/KERNEL_6.14_FIX_README.md)
- Testing: [docs/TESTING_GUIDE.md](docs/TESTING_GUIDE.md)

### Official Support
- For official driver updates, contact Quixant
- Request kernel 6.14+ native driver support

### Community Patches
These kernel 6.14+ patches are community-developed to bridge the gap until official support is available.

---

## License

Original QMI4L software © Quixant Limited
Kernel compatibility patches © 2025

---

## Credits

- **Original QMI4L:** Quixant Limited
- **Kernel 6.6+ Patches:** Community
- **Kernel 6.14+ Enhancements:** Extended compatibility (2025-12-02)
- **Documentation:** Comprehensive guides and references

---

**Ready to install? Run `sudo ./qmi4l -s`**

For detailed information, see [README_KERNEL_6.14.txt](README_KERNEL_6.14.txt)
