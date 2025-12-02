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
