# Quixant Driver Installation - Testing Guide

**Version:** 2.0.2.3-k6.6
**Purpose:** Verify successful installation of Quixant drivers on Linux kernel 6.6+

---

## Quick Status Check

### Single Command Test
```bash
sudo ./qmi4l -p
```
This displays complete system status including drivers, services, and ports.

---

## Detailed Testing Procedures

### 1. Verify Kernel Modules Loaded

**Command:**
```bash
lsmod | grep -E 'qxt|qli|tracer'
```

**Expected Output:**
```
qxtpch          16384  0
drvtracer       20480  0
qxtsecs         32768  0
qli2            45056  0
qxtnvram        28672  0
qxtio           36864  0
```

**✅ PASS:** All 6 modules listed
**❌ FAIL:** Any module missing

---

### 2. Check Service Status

**Commands:**
```bash
systemctl status qxtDrv
systemctl status rc-local
```

**Expected Output:**
```
● qxtDrv.service - LSB: QUIXANT drivers service
   Loaded: loaded (/etc/init.d/qxtDrv; generated)
   Active: active (exited) since [timestamp]

● rc-local.service - /etc/rc.local Compatibility
   Loaded: loaded (/etc/systemd/system/rc-local.service; enabled)
   Active: active (exited) since [timestamp]
```

**✅ PASS:** Both services show "active (exited)"
**❌ FAIL:** Services show "failed" or "inactive"

---

### 3. Verify Driver Files Installed

**Command:**
```bash
ls -lh /opt/quixant/drivers/
```

**Expected Output:**
```
-rw-r--r-- 1 root root  21K drvtracer-2.3.1.0.ko
lrwxrwxrwx 1 root root   22 drvtracer.ko -> drvtracer-2.3.1.0.ko
-rw-r--r-- 1 root root  45K qli2-4.5.0.1.ko
lrwxrwxrwx 1 root root   16 qli2.ko -> qli2-4.5.0.1.ko
-rw-r--r-- 1 root root  37K qxtio-0.7.0.1.ko
lrwxrwxrwx 1 root root   16 qxtio.ko -> qxtio-0.7.0.1.ko
-rw-r--r-- 1 root root  29K qxtnvram-3.9.2.5.ko
lrwxrwxrwx 1 root root   20 qxtnvram.ko -> qxtnvram-3.9.2.5.ko
-rw-r--r-- 1 root root  17K qxtpch-1.3.0.0.ko
lrwxrwxrwx 1 root root   18 qxtpch.ko -> qxtpch-1.3.0.0.ko
-rw-r--r-- 1 root root  33K qxtsecs-1.6.0.1.ko
lrwxrwxrwx 1 root root   19 qxtsecs.ko -> qxtsecs-1.6.0.1.ko
```

**✅ PASS:** 12 files total (6 .ko files + 6 symlinks)
**❌ FAIL:** Files missing or directory doesn't exist

---

### 4. Test Module Information

**Commands:**
```bash
modinfo /opt/quixant/drivers/qxtio.ko
modinfo /opt/quixant/drivers/qli2.ko
modinfo /opt/quixant/drivers/qxtsecs.ko
```

**Expected Output (example for qxtio):**
```
filename:       /opt/quixant/drivers/qxtio-0.7.0.1.ko
version:        0.7.0.1
description:    Quixant Gaming IO Driver
author:         Marco Barbini <Marco.Barbini@quixant.com>
license:        GPL
vermagic:       6.14.0-34-generic SMP preempt mod_unload modversions
```

**✅ PASS:** All commands return valid module info
**❌ FAIL:** "Module not found" or other errors

---

### 5. Verify Serial Ports

**Commands:**
```bash
ls -l /dev/ttyS[0-5]
stty -F /dev/ttyS0
```

**Expected Output:**
```
crw-rw---- 1 root dialout 4, 64 Nov  3 17:49 /dev/ttyS0
crw-rw---- 1 root dialout 4, 65 Nov  3 17:49 /dev/ttyS1
crw-rw---- 1 root dialout 4, 66 Nov  3 17:49 /dev/ttyS2
crw-rw---- 1 root dialout 4, 67 Nov  3 17:49 /dev/ttyS3
crw-rw---- 1 root dialout 4, 68 Nov  3 17:49 /dev/ttyS4
crw-rw---- 1 root dialout 4, 69 Nov  3 17:49 /dev/ttyS5

speed 9600 baud; line = 0;
-brkint -imaxbel
```

**✅ PASS:** All 6 ports exist and stty runs without error
**❌ FAIL:** Ports missing or permission denied

---

### 6. Check Hardware Detection

**Command:**
```bash
lspci -n | grep 19d4
```

**Expected Output:**
```
01:00.0 0904: 19d4:0100 (rev 01)  # CORE
01:00.1 0580: 19d4:0200 (rev 01)  # NVRAM
01:00.2 0700: 19d4:0a00 (rev 01)  # (Other)
01:00.3 0380: 19d4:0e00 (rev 01)  # QLI2
01:00.4 1010: 19d4:0300 (rev 01)  # SECS
01:00.5 0780: 19d4:0c00 (rev 01)  # TRACER
```

**✅ PASS:** Quixant devices detected
**❌ FAIL:** No devices with vendor ID 19d4

---

### 7. Verify udev Rules

**Command:**
```bash
cat /etc/udev/rules.d/91-quixant.rules
```

**Expected Output:**
```
# Quixant Logging Processor (Atmel)
SUBSYSTEM=="usb", ATTR{idVendor}=="03eb", ATTR{idProduct}=="7512", MODE:="666", GROUP="dialout"
# Quixant QB029 Led Board (Atmel)
SUBSYSTEM=="usb", ATTR{idVendor}=="03eb", ATTR{idProduct}=="7513", MODE:="666", GROUP="dialout"
...
```

**✅ PASS:** File exists with Quixant USB rules
**❌ FAIL:** File missing or empty

---

### 8. Check System Logs

**Commands:**
```bash
dmesg | grep -i quixant | tail -20
journalctl -u qxtDrv -n 20 --no-pager
```

**Expected Output:**
```
[timestamp] qxtio: loading driver...
[timestamp] qxtnvram: loading driver...
[timestamp] qli2: loading driver...
...
```

**✅ PASS:** No error messages, only initialization logs
**❌ FAIL:** Error messages present

---

### 9. User Permissions Check

**Command:**
```bash
groups quixant
```

**Expected Output:**
```
quixant : quixant dialout plugdev ...
```

**✅ PASS:** User belongs to dialout and plugdev groups
**❌ FAIL:** Missing required groups

---

### 10. Verify Installation Version

**Command:**
```bash
./qmi4l -h | grep Version
```

**Expected Output:**
```
║          Version 2.0.2.3-k6.6              ║
```

**✅ PASS:** Version shows "2.0.2.3-k6.6" (kernel 6.6+ compatible)
**❌ FAIL:** Different version or original "2.0.2.3"

---

## Automated Test Script

Save this as `test_quixant.sh`:

```bash
#!/bin/bash
# Automated Quixant Driver Testing Script

echo "======================================"
echo "Quixant Driver Installation Test"
echo "======================================"
echo

PASS=0
FAIL=0

# Test 1: Modules loaded
echo -n "Test 1: Checking kernel modules........."
if [ $(lsmod | grep -E 'qxt|qli|tracer' | wc -l) -eq 6 ]; then
    echo "PASS"
    ((PASS++))
else
    echo "FAIL"
    ((FAIL++))
fi

# Test 2: Services active
echo -n "Test 2: Checking services..............."
if systemctl is-active --quiet qxtDrv && systemctl is-active --quiet rc-local; then
    echo "PASS"
    ((PASS++))
else
    echo "FAIL"
    ((FAIL++))
fi

# Test 3: Driver files exist
echo -n "Test 3: Checking driver files.........."
if [ -d /opt/quixant/drivers ] && [ $(ls /opt/quixant/drivers/*.ko 2>/dev/null | wc -l) -eq 6 ]; then
    echo "PASS"
    ((PASS++))
else
    echo "FAIL"
    ((FAIL++))
fi

# Test 4: Serial ports accessible
echo -n "Test 4: Checking serial ports.........."
if [ -c /dev/ttyS0 ] && [ -c /dev/ttyS5 ]; then
    echo "PASS"
    ((PASS++))
else
    echo "FAIL"
    ((FAIL++))
fi

# Test 5: udev rules exist
echo -n "Test 5: Checking udev rules............."
if [ -f /etc/udev/rules.d/91-quixant.rules ]; then
    echo "PASS"
    ((PASS++))
else
    echo "FAIL"
    ((FAIL++))
fi

# Test 6: User permissions
echo -n "Test 6: Checking user permissions......"
if groups quixant | grep -q dialout && groups quixant | grep -q plugdev; then
    echo "PASS"
    ((PASS++))
else
    echo "FAIL"
    ((FAIL++))
fi

echo
echo "======================================"
echo "Results: $PASS passed, $FAIL failed"
echo "======================================"

if [ $FAIL -eq 0 ]; then
    echo "✓ All tests PASSED - Installation OK"
    exit 0
else
    echo "✗ Some tests FAILED - Check output"
    exit 1
fi
```

**Usage:**
```bash
chmod +x test_quixant.sh
sudo ./test_quixant.sh
```

---

## Troubleshooting Failed Tests

### Modules Not Loaded
```bash
# Manually load modules
sudo modprobe qxtio
sudo modprobe qxtnvram
sudo modprobe qli2
sudo modprobe qxtsecs
sudo modprobe drvtracer
sudo modprobe qxtpch

# Check for errors
dmesg | tail -20
```

### Services Not Active
```bash
# Restart services
sudo systemctl restart qxtDrv
sudo systemctl restart rc-local

# Check logs
journalctl -u qxtDrv -n 50
```

### Permission Issues
```bash
# Add user to groups
sudo usermod -a -G dialout quixant
sudo usermod -a -G plugdev quixant

# Re-login required for group changes
```

### Complete Reinstall
```bash
# Uninstall
sudo ./qmi4l -u

# Clean reinstall
sudo ./qmi4l -s

# Verify
sudo ./qmi4l -p
```

---

## Test Results Checklist

| Test | Status | Notes |
|------|--------|-------|
| Kernel modules loaded | ☐ | 6 modules required |
| Services active | ☐ | qxtDrv + rc-local |
| Driver files present | ☐ | 12 files in /opt/quixant/drivers |
| Module info readable | ☐ | modinfo commands work |
| Serial ports accessible | ☐ | 6 ports (ttyS0-5) |
| Hardware detected | ☐ | lspci shows 19d4 devices |
| udev rules configured | ☐ | 91-quixant.rules exists |
| No errors in logs | ☐ | dmesg/journalctl clean |
| User permissions OK | ☐ | dialout + plugdev groups |
| Correct version installed | ☐ | 2.0.2.3-k6.6 |

---

## Success Criteria

**Installation is SUCCESSFUL if:**
- ✅ All 6 kernel modules loaded
- ✅ Both services show "active (exited)"
- ✅ All driver files present in `/opt/quixant/drivers/`
- ✅ Serial ports accessible without errors
- ✅ No error messages in system logs

**Installation FAILED if:**
- ❌ Any kernel module missing from `lsmod`
- ❌ Services in "failed" or "inactive" state
- ❌ Driver files missing
- ❌ Serial ports inaccessible
- ❌ Error messages in `dmesg` or `journalctl`

---

## Post-Installation

After successful testing:

1. **Reboot test** (optional but recommended):
   ```bash
   sudo reboot
   # After reboot:
   sudo ./qmi4l -p
   ```

2. **Document results:**
   - Save test output to file
   - Note any warnings or issues
   - Record kernel version and hardware model

3. **Backup working configuration:**
   ```bash
   tar -czf quixant-working-config.tar.gz /opt/quixant /etc/udev/rules.d/91-quixant.rules
   ```

---

**Document Version:** 1.0
**Last Updated:** November 3, 2025
**Compatible With:** qmi4l 2.0.2.3-k6.6 (Kernel 6.6+ compatible)
