# Utility Scripts Documentation

This directory contains utility scripts for working with Quixant drivers and the installer.

---

## Scripts Overview

| Script | Purpose | Usage |
|--------|---------|-------|
| [patch_all_drivers.sh](#patch_all_driverssh) | Patches all drivers for kernel 6.6+ | For manual patching |
| [patch_and_repackage.sh](#patch_and_repackagesh) | Patches and repackages driver archives | Development use |
| [create_driverscomp_wrapper.sh](#create_driverscomp_wrappersh) | Creates wrapper scripts | Development use |
| [fix_permissions_and_patch.sh](#fix_permissions_and_patchsh) | Fixes permissions and patches | Setup utility |
| [test_quixant.sh](#test_quixantsh) | Tests Quixant hardware detection | Hardware testing |

---

## patch_all_drivers.sh

### Description
Batch patches all Quixant drivers for Linux kernel 6.6+ compatibility by replacing the removed `asm/unaligned.h` header with `linux/unaligned.h`.

### Purpose
- Fixes kernel 6.6+ compilation issues
- Creates wrapper scripts for driverscomp binaries
- Patches drivers that are already extracted

### Usage

```bash
cd /path/to/QuixantModuleInstaller
./scripts/patch_all_drivers.sh
```

### What It Does

1. **Locates Drivers**: Finds all extracted driver directories in `drivers/QXi-7000_20240730/`
2. **Creates Wrappers**: For each driver with a `driverscomp` binary, creates a wrapper script
3. **Patches on Compilation**: The wrapper automatically patches source files when compilation is triggered

### When to Use

- **Manual patching**: If you've extracted drivers manually and need to patch them
- **Development**: Testing kernel 6.6 compatibility before using the main installer
- **Troubleshooting**: If automatic patching in qmi4l isn't working

### Note

⚠️ **Not needed for normal installation!** The main `qmi4l` installer automatically handles all patching. This script is for manual/development use only.

---

## patch_and_repackage.sh

### Description
Advanced script that extracts driver archives, patches them, and repackages them with the patches included.

### Purpose
- Development and testing of driver patches
- Creating pre-patched driver archives
- Preparing drivers for distribution

### Usage

```bash
cd /path/to/QuixantModuleInstaller
./scripts/patch_and_repackage.sh
```

### What It Does

1. **Extracts** each driver `.tgz` archive
2. **Creates** backup of original archive
3. **Patches** driverscomp to include wrapper
4. **Repackages** into new `.tgz` with patches
5. **Preserves** original as `.tgz.orig`

### When to Use

- **Creating pre-patched drivers**: For systems without build tools
- **Testing patches**: Verify patches work before integration
- **Distribution**: Prepare patched drivers for others

### Output

```
drivers/
├── R_DRV_CORE_LIN_x64_0.7.0.1.tgz         (patched)
├── R_DRV_CORE_LIN_x64_0.7.0.1.tgz.orig    (original backup)
├── R_DRV_NVRAM_LIN_x64_3.9.2.5.tgz        (patched)
├── R_DRV_NVRAM_LIN_x64_3.9.2.5.tgz.orig   (original backup)
...
```

### Note

⚠️ **For development use!** Normal users should use the main `qmi4l` installer which handles patching automatically.

---

## create_driverscomp_wrapper.sh

### Description
Creates wrapper scripts for `driverscomp` binaries that intercept make calls to patch source files on-the-fly.

### Purpose
- Generate patching wrappers for driver compilation
- Automate header replacement during build
- Development and testing of wrapper logic

### Usage

```bash
cd /path/to/QuixantModuleInstaller
./scripts/create_driverscomp_wrapper.sh
```

### What It Does

1. **Scans** for all driver `.tgz` archives
2. **Extracts** each archive temporarily
3. **Creates** a custom wrapper for driverscomp binary
4. **Repackages** drivers with wrapper included

### Wrapper Functionality

The created wrapper:
- Backs up original `driverscomp` as `driverscomp.orig`
- Creates custom `make` command in PATH
- Patches `asm/unaligned.h` to `linux/unaligned.h` before compilation
- Runs original driverscomp
- Cleans up temporary files

### When to Use

- **Wrapper development**: Testing new wrapper approaches
- **Pre-patching archives**: Create wrapper-enabled driver packages
- **Understanding mechanics**: See how wrapper creation works

### Note

⚠️ **Development tool!** The main `qmi4l` installer creates wrappers automatically during installation.

---

## fix_permissions_and_patch.sh

### Description
Utility script to fix file permissions and apply patches to the installer or driver files.

### Purpose
- Fix permission issues with driver archives
- Apply necessary patches to qmi4l
- Prepare system for installation

### Usage

```bash
cd /path/to/QuixantModuleInstaller
./scripts/fix_permissions_and_patch.sh
```

### What It Does

1. **Fixes permissions** on driver `.tgz` files
2. **Applies patches** to qmi4l if needed
3. **Sets execute** permissions on scripts
4. **Prepares** environment for installation

### When to Use

- **Permission errors**: If you get "Permission denied" errors
- **After extraction**: When driver archives have wrong permissions
- **Setup**: Before running qmi4l for the first time

### Common Issues It Fixes

- Driver archives owned by root
- Missing execute permissions on qmi4l
- Incorrect permissions on utility scripts

---

## test_quixant.sh

### Description
Hardware detection and testing script that verifies Quixant hardware presence and driver compatibility.

### Purpose
- Detect Quixant PCI devices
- Verify hardware before installation
- Test driver loading

### Usage

```bash
cd /path/to/QuixantModuleInstaller
./scripts/test_quixant.sh
```

### What It Does

1. **Scans PCI bus** for Quixant devices (vendor ID: 19d4)
2. **Lists detected** devices with IDs and descriptions
3. **Shows** which drivers are needed
4. **Reports** installation readiness

### Sample Output

```
Scanning for Quixant devices...

Found Quixant devices:
  01:00.0 Gameport controller [19d4:0100] - CORE
  01:00.1 Memory controller [19d4:0200] - NVRAM
  01:00.2 Serial controller [19d4:0a00] - Serial
  01:00.4 Encryption device [19d4:0300] - SECS
  01:00.5 Communication controller [19d4:0c00] - TRACER

Required drivers:
  ✓ R_DRV_CORE_LIN_x64_0.7.0.1.tgz
  ✓ R_DRV_NVRAM_LIN_x64_3.9.2.5.tgz
  ✓ R_DRV_QXTSECS_LIN_x64_1.6.0.1.tgz
  ✓ R_DRV_TRACER_LIN_x64_2.3.1.0.tgz

System ready for installation.
```

### When to Use

- **Pre-installation**: Verify hardware before running qmi4l
- **Troubleshooting**: Check if hardware is detected
- **Hardware verification**: Confirm PCI device IDs

### Exit Codes

- `0` - Quixant hardware detected
- `1` - No Quixant hardware found
- `2` - Missing required tools (lspci)

---

## Script Dependencies

All scripts require:

- **bash** - Shell interpreter
- **Standard tools**: `sed`, `grep`, `tar`, `find`, `ls`
- **For test_quixant.sh**: `lspci`

Most dependencies are pre-installed on Ubuntu systems.

---

## Development Workflow

### Typical Development Sequence

1. **Test hardware detection**:
   ```bash
   ./scripts/test_quixant.sh
   ```

2. **Fix permissions if needed**:
   ```bash
   ./scripts/fix_permissions_and_patch.sh
   ```

3. **Test manual patching** (optional):
   ```bash
   ./scripts/patch_all_drivers.sh
   ```

4. **Create pre-patched archives** (optional):
   ```bash
   ./scripts/patch_and_repackage.sh
   ```

5. **Install with main installer**:
   ```bash
   sudo ./qmi4l -s
   ```

---

## Important Notes

### For Normal Users

⚠️ **You don't need these scripts!**

Just run:
```bash
sudo ./qmi4l -s
```

The main installer handles all patching automatically.

### For Developers

These scripts are useful for:
- **Testing** patches before integration
- **Understanding** how automatic patching works
- **Creating** pre-patched driver packages
- **Debugging** installation issues
- **Development** of new compatibility fixes

---

## Script Locations

All scripts are in the `scripts/` directory:

```
scripts/
├── README.md                          (this file)
├── patch_all_drivers.sh               Batch driver patcher
├── patch_and_repackage.sh             Archive repackager
├── create_driverscomp_wrapper.sh      Wrapper generator
├── fix_permissions_and_patch.sh       Permission fixer
└── test_quixant.sh                    Hardware tester
```

---

## Troubleshooting

### Permission Denied

```bash
chmod +x scripts/*.sh
./scripts/fix_permissions_and_patch.sh
```

### Script Not Found

Ensure you're in the repository root:
```bash
cd /path/to/QuixantModuleInstaller-QXi-7000-20240730
./scripts/script_name.sh
```

### Patches Don't Apply

The scripts expect a specific directory structure. Ensure:
- You're in the repository root
- Driver archives are in `drivers/`
- Extracted drivers are in `drivers/QXi-7000_20240730/`

---

## Related Documentation

- **Main README**: [../README.md](../README.md)
- **Installation Guide**: [../INSTALLATION_READY.txt](../INSTALLATION_READY.txt)
- **Kernel 6.14 Fixes**: [../docs/KERNEL_6.14_FIX_README.md](../docs/KERNEL_6.14_FIX_README.md)
- **Quick Reference**: [../docs/KERNEL_6.14_QUICK_REFERENCE.txt](../docs/KERNEL_6.14_QUICK_REFERENCE.txt)

---

## Support

For issues with these scripts:

1. Check this documentation
2. Verify you're using the correct kernel version: `uname -r`
3. Read the main installer help: `./qmi4l -h`
4. Review troubleshooting: [../docs/KERNEL_6.14_QUICK_REFERENCE.txt](../docs/KERNEL_6.14_QUICK_REFERENCE.txt)

---

**Remember**: For normal installation, just use `sudo ./qmi4l -s` - these scripts are development utilities!
