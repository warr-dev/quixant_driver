# Quixant Example Programs

This directory contains example programs demonstrating how to interact with Quixant hardware devices.

---

## Quick Start

```bash
# Build all examples
make

# Run basic test
make test

# Run CORE I/O demo
make demo

# Clean up
make clean
```

---

## Available Examples

| Program | Language | Purpose | Devices |
|---------|----------|---------|---------|
| [test_qxtio.c](#test_qxtioc) | C | Basic device test | All Quixant devices |
| [core_io_example.c](#core_io_examplec) | C | CORE I/O operations | CORE device |
| [button_monitor.c](#button_monitorc) | C | Button input monitoring | Input buttons |
| [test_qxtio_buttons.c](#test_qxtio_buttonsc) | C | Button testing | Input buttons |
| [test_qxtio_live.c](#test_qxtio_livec) | C | Live device monitoring | All devices |
| [io_quixant.cpp/h](#io_quixantcpp) | C++ | C++ interface wrapper | All devices |

---

## test_qxtio.c

### Description
Basic test program for Quixant devices. Tests fundamental device operations and verifies driver installation.

### Features
- Device detection and enumeration
- Basic read/write operations
- Device status checking
- Error handling examples

### Building

```bash
make test_qxtio
# or
gcc -Wall -Wextra -O2 -o test_qxtio examples/test_qxtio.c
```

### Usage

```bash
./test_qxtio
```

### Output

```
Quixant Device Test Program
============================

Scanning for devices...
Found: /dev/qxtcore0
Found: /dev/qxtnvram0
Found: /dev/qxtsecs0

Testing /dev/qxtcore0...
  ✓ Open successful
  ✓ Read test passed
  ✓ Write test passed
  ✓ Close successful

All tests passed!
```

### When to Use
- **After installation**: Verify drivers are working
- **Troubleshooting**: Check device accessibility
- **Learning**: Understand basic device operations

### Device Files Accessed
- `/dev/qxtcore*` - CORE device
- `/dev/qxtnvram*` - NVRAM device
- `/dev/qxtsecs*` - SECS device
- `/dev/qxttracer*` - TRACER device

---

## core_io_example.c

### Description
Comprehensive example demonstrating CORE device I/O operations including GPIO, interrupts, and timers.

### Features
- GPIO pin control (input/output)
- Interrupt handling
- Timer operations
- Status LED control
- Error handling and recovery

### Building

```bash
make core_io_example
# or
gcc -Wall -Wextra -O2 -o core_io_example examples/core_io_example.c
```

### Usage

```bash
# Run with default options
./core_io_example

# Run with specific test
./core_io_example --gpio
./core_io_example --timer
./core_io_example --led
```

### Example Operations

```c
// Open CORE device
int fd = open("/dev/qxtcore0", O_RDWR);

// Set GPIO pin
ioctl(fd, QXT_GPIO_SET, pin_number);

// Read GPIO state
ioctl(fd, QXT_GPIO_GET, &state);

// Configure timer
ioctl(fd, QXT_TIMER_SET, &timer_config);

// Control LED
ioctl(fd, QXT_LED_CTRL, &led_state);
```

### When to Use
- **GPIO operations**: Learn how to control GPIO pins
- **Timer programming**: Understand timer configuration
- **LED control**: Implement status indicators
- **Advanced I/O**: Complex device interactions

### Requirements
- CORE driver loaded
- Appropriate permissions (usually requires root or dialout group)

---

## button_monitor.c

### Description
Real-time button monitoring program. Continuously monitors button state and reports press/release events.

### Features
- Continuous button state monitoring
- Press/release event detection
- Debouncing logic
- Multi-button support

### Building

```bash
gcc -Wall -Wextra -O2 -o button_monitor examples/button_monitor.c
```

### Usage

```bash
./button_monitor
```

### Output

```
Button Monitor Starting...
Monitoring buttons (Press Ctrl+C to exit)

[12:34:56] Button 1 PRESSED
[12:34:57] Button 1 RELEASED
[12:34:58] Button 2 PRESSED
[12:34:58] Button 3 PRESSED
[12:34:59] Button 2 RELEASED
```

### When to Use
- **Button testing**: Verify button hardware works
- **Input debugging**: Check button response
- **Event handling**: Learn event-driven programming
- **Game development**: Implement button controls

### Button Mapping
Depends on your specific Quixant hardware. Common mappings:
- Button 1: Service button
- Button 2: Test button
- Button 3-N: Game buttons

---

## test_qxtio_buttons.c

### Description
Dedicated button testing utility with comprehensive test suite for all button functions.

### Features
- Individual button testing
- Multi-button press detection
- Button combination testing
- Timing analysis
- Auto-repeat testing

### Building

```bash
gcc -Wall -Wextra -O2 -o test_qxtio_buttons examples/test_qxtio_buttons.c
```

### Usage

```bash
./test_qxtio_buttons

# Run specific test
./test_qxtio_buttons --test-single
./test_qxtio_buttons --test-combo
./test_qxtio_buttons --test-timing
```

### Tests Performed

1. **Single Button Test**: Press each button individually
2. **Combination Test**: Test multiple simultaneous presses
3. **Timing Test**: Measure press duration and response time
4. **Auto-repeat Test**: Verify repeat functionality
5. **All Buttons Test**: Stress test all buttons

### When to Use
- **Quality assurance**: Verify button hardware
- **Manufacturing**: Production testing
- **Diagnostics**: Identify button issues
- **Calibration**: Set button parameters

---

## test_qxtio_live.c

### Description
Live monitoring and diagnostic tool. Provides real-time display of all Quixant device states.

### Features
- Real-time device status
- Continuous monitoring
- Multiple device support
- Color-coded output
- Refresh rate control

### Building

```bash
gcc -Wall -Wextra -O2 -o test_qxtio_live examples/test_qxtio_live.c
```

### Usage

```bash
# Default refresh rate (1 second)
./test_qxtio_live

# Custom refresh rate (100ms)
./test_qxtio_live --refresh 100
```

### Display

```
Quixant Live Monitor - Refresh: 1000ms
========================================

CORE Device:
  Status: Active
  Temp: 45°C
  Voltage: 12.1V

NVRAM Device:
  Status: Active
  Size: 8192 bytes
  Used: 2048 bytes

SECS Device:
  Status: Active
  Seed: 0x12345678

[Press Ctrl+C to exit]
```

### When to Use
- **System monitoring**: Watch device health
- **Debugging**: Track state changes
- **Performance**: Monitor system metrics
- **Development**: Real-time feedback

---

## io_quixant.cpp / io_quixant.h

### Description
C++ wrapper classes for Quixant devices. Provides object-oriented interface with modern C++ features.

### Features
- RAII resource management
- Exception-safe operations
- Type-safe interfaces
- Iterator support
- Standard C++ idioms

### Class Overview

```cpp
// Base device class
class QxtDevice {
public:
    QxtDevice(const std::string& device_path);
    ~QxtDevice();

    bool open();
    void close();
    bool isOpen() const;

    ssize_t read(void* buffer, size_t size);
    ssize_t write(const void* buffer, size_t size);

    int ioctl(unsigned long request, void* arg);
};

// CORE device
class QxtCore : public QxtDevice {
public:
    void setGPIO(int pin, bool state);
    bool getGPIO(int pin);
    void setLED(int led, bool state);
    // ...
};

// NVRAM device
class QxtNVRAM : public QxtDevice {
public:
    void write(size_t offset, const void* data, size_t size);
    void read(size_t offset, void* data, size_t size);
    size_t capacity() const;
    // ...
};
```

### Building with C++

```bash
g++ -Wall -Wextra -O2 -std=c++11 -o myapp myapp.cpp examples/io_quixant.cpp
```

### Example Usage

```cpp
#include "examples/io_quixant.h"

int main() {
    try {
        // Create CORE device
        QxtCore core("/dev/qxtcore0");

        // Open device
        if (!core.open()) {
            std::cerr << "Failed to open device\n";
            return 1;
        }

        // Use RAII - auto cleanup
        {
            // Set GPIO
            core.setGPIO(1, true);

            // Control LED
            core.setLED(0, true);

            // Read status
            auto status = core.getStatus();
            std::cout << "Status: " << status << "\n";

        } // Device automatically closed here

        return 0;

    } catch (const QxtException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
```

### When to Use
- **C++ projects**: Modern C++ interface
- **Exception safety**: RAII resource management
- **Type safety**: Compile-time checks
- **Integration**: Easy to integrate into C++ codebases

---

## Common Patterns

### Error Handling

```c
// C style
int fd = open("/dev/qxtcore0", O_RDWR);
if (fd < 0) {
    perror("open");
    return -1;
}

// Check ioctl results
if (ioctl(fd, CMD, &arg) < 0) {
    perror("ioctl");
    close(fd);
    return -1;
}

close(fd);
```

### Permission Requirements

Most operations require elevated privileges or group membership:

```bash
# Run as root
sudo ./test_qxtio

# Or add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and back in, then:
./test_qxtio
```

### Device Paths

Standard Quixant device paths:
```c
#define QXT_CORE_DEV    "/dev/qxtcore0"
#define QXT_NVRAM_DEV   "/dev/qxtnvram0"
#define QXT_SECS_DEV    "/dev/qxtsecs0"
#define QXT_TRACER_DEV  "/dev/qxttracer0"
```

---

## Building All Examples

### Using Makefile (Recommended)

```bash
# From repository root
make                # Build all
make test_qxtio     # Build specific
make clean          # Clean up
```

### Manual Compilation

```bash
# C programs
gcc -Wall -Wextra -O2 -o test_qxtio examples/test_qxtio.c
gcc -Wall -Wextra -O2 -o core_io_example examples/core_io_example.c

# C++ programs
g++ -Wall -Wextra -O2 -std=c++11 -o myapp myapp.cpp examples/io_quixant.cpp
```

### Compilation Options

- `-Wall -Wextra`: Enable warnings
- `-O2`: Optimization level 2
- `-g`: Add debug symbols
- `-std=c++11`: C++11 standard (for C++)

---

## Testing Workflow

### 1. Basic Testing

```bash
# Build and run basic test
make test
```

### 2. Device Testing

```bash
# Test each device type
./test_qxtio
```

### 3. Interactive Testing

```bash
# Monitor devices in real-time
./test_qxtio_live
```

### 4. Button Testing

```bash
# Test button inputs
./test_qxtio_buttons
```

### 5. Advanced Testing

```bash
# Run CORE I/O demo
make demo
```

---

## Troubleshooting

### Device Not Found

```bash
# Check if drivers are loaded
lsmod | grep qxt

# Check device nodes exist
ls -l /dev/qxt*
```

### Permission Denied

```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER

# Or run as root
sudo ./test_qxtio
```

### Compilation Errors

```bash
# Install build tools
sudo apt install build-essential

# Check gcc version
gcc --version
```

### Device Busy

```bash
# Check what's using the device
lsof | grep qxt

# Kill the process or wait for it to finish
```

---

## Development Guide

### Creating New Examples

1. **Copy template**: Start with `test_qxtio.c` as base
2. **Add features**: Implement your test/demo logic
3. **Update Makefile**: Add build target
4. **Test**: Verify on real hardware
5. **Document**: Add to this README

### Best Practices

- **Check return values**: Always check for errors
- **Close resources**: Use proper cleanup
- **Handle signals**: Implement Ctrl+C handling
- **Add comments**: Document complex logic
- **Test thoroughly**: Verify on target hardware

---

## Requirements

### System Requirements
- Ubuntu 18.04+ (or compatible Linux)
- Quixant drivers installed
- Build tools (`gcc`/`g++`)

### Hardware Requirements
- Quixant QXi-7000 or compatible
- Appropriate PCI devices
- Proper power supply

### Permissions
- Root access or dialout group membership
- Read/write access to `/dev/qxt*` devices

---

## Related Documentation

- **Main README**: [../README.md](../README.md)
- **Installation**: [../INSTALLATION_READY.txt](../INSTALLATION_READY.txt)
- **API Documentation**: [../docs/COMPLETE_DOCUMENTATION.md](../docs/COMPLETE_DOCUMENTATION.md)
- **Testing Guide**: [../docs/TESTING_GUIDE.md](../docs/TESTING_GUIDE.md)

---

## Support

For issues with example programs:

1. Verify drivers are installed: `sudo ./qmi4l -p`
2. Check device permissions: `ls -l /dev/qxt*`
3. Test with basic example: `make test`
4. Review error messages carefully
5. Check system logs: `dmesg | grep qxt`

---

**Happy coding!** These examples provide a foundation for developing applications with Quixant hardware.
