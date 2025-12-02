/*
 * core_io_example.c - Example program for Quixant CORE I/O driver
 *
 * This program demonstrates direct interaction with /dev/qxtio device
 * for reading inputs (buttons, doors, intrusions) and controlling
 * outputs (LEDs, locks, relays) on Quixant QX7000 hardware.
 *
 * Based on libDrivers.h API definitions but using direct IOCTL access
 * since the compiled libDrivers.so library is not available.
 *
 * Compile: gcc -o core_io_example core_io_example.c
 * Run: ./core_io_example
 *
 * Hardware: QXi-7000Lite
 * Driver: qxtio v0.7.0.1
 * Device: /dev/qxtio (234:0)
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>

#define DEVICE_PATH "/dev/qxtio"

// IOCTL command definitions from Quixant SDK
// These values are from libDrivers.h and Quixant documentation
#define QXT_GET_VERSION         0x1001
#define QXT_I2C_RW              0x1002
#define QXT_I2C_LOCK            0x1003
#define QXT_I2C_UNLOCK          0x1004
#define QXT_I2C_SET_BUS         0x1005
#define QXT_CFG_REG_GET         0x1006
#define QXT_CFG_REG_SET         0x1007

// Custom IOCTL commands for I/O operations
// Note: Actual command codes may differ - check qxtio driver source
#define QXT_GET_INPUT_MASK      0x2001
#define QXT_GET_OUTPUT_MASK     0x2002
#define QXT_SET_OUTPUT_MASK     0x2003
#define QXT_SET_OUTPUT_BIT      0x2004
#define QXT_CLEAR_OUTPUT_BIT    0x2005

// Input bit definitions (from io_quixant.h)
#define QX_INPUT_DOOR_START     18
#define QX_INPUT_DOOR_END       21

// Global flag for signal handling
static volatile int keep_running = 1;

// Signal handler for graceful shutdown
void signal_handler(int signum) {
    printf("\n\nReceived signal %d (Ctrl+C), shutting down...\n", signum);
    keep_running = 0;
}

// Print binary representation of a 32-bit value
void print_binary(uint32_t value, const char *label) {
    printf("%s: ", label);
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf(" (0x%08x)\n", value);
}

// Monitor input changes
void monitor_inputs(int fd) {
    uint32_t last_inputs = 0;
    uint32_t current_inputs = 0;
    int first_read = 1;
    int count = 0;

    printf("\n========================================\n");
    printf("MONITORING INPUT STATES\n");
    printf("========================================\n");
    printf("Watching for button presses, door opens, intrusions...\n");
    printf("Press Ctrl+C to stop\n");
    printf("----------------------------------------\n\n");

    while (keep_running) {
        // Try to read input mask using IOCTL
        int result = ioctl(fd, QXT_GET_INPUT_MASK, &current_inputs);

        if (result < 0) {
            // IOCTL not supported, try direct read instead
            ssize_t bytes = read(fd, &current_inputs, sizeof(current_inputs));
            if (bytes < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    usleep(100000); // 100ms
                    continue;
                }
                perror("read() error");
                break;
            }
        }

        // Show heartbeat
        printf("[Count: %d] Monitoring inputs...\r", count++);
        fflush(stdout);

        // Check for changes
        if (first_read) {
            first_read = 0;
            last_inputs = current_inputs;
            printf("\nInitial input state:\n");
            print_binary(current_inputs, "Inputs");
            printf("\n");
        } else if (current_inputs != last_inputs) {
            uint32_t changed = current_inputs ^ last_inputs;
            uint32_t rising = changed & current_inputs;
            uint32_t falling = changed & ~current_inputs;

            printf("\n*** INPUT CHANGE DETECTED ***\n");
            print_binary(current_inputs, "Current ");
            print_binary(last_inputs, "Previous");
            print_binary(changed, "Changed ");
            print_binary(rising, "Rising  ");
            print_binary(falling, "Falling ");

            // Decode specific inputs
            printf("\nDecoded changes:\n");
            for (int bit = 0; bit < 32; bit++) {
                if (changed & (1 << bit)) {
                    const char *state = (rising & (1 << bit)) ? "HIGH" : "LOW";

                    // Check if it's a door input
                    if (bit >= QX_INPUT_DOOR_START && bit <= QX_INPUT_DOOR_END) {
                        int door_num = bit - QX_INPUT_DOOR_START + 1;
                        printf("  Bit %2d: DOOR %d changed to %s\n", bit, door_num, state);
                    } else {
                        printf("  Bit %2d: Changed to %s\n", bit, state);
                    }
                }
            }
            printf("\n");

            last_inputs = current_inputs;
        }

        usleep(100000); // 100ms polling
    }

    printf("\n\nInput monitoring stopped.\n");
}

// Test output control
void test_outputs(int fd) {
    printf("\n========================================\n");
    printf("TESTING OUTPUT CONTROL\n");
    printf("========================================\n\n");

    uint32_t output_mask = 0;
    int result;

    // Try to get current output state
    printf("Test 1: Reading current output state...\n");
    result = ioctl(fd, QXT_GET_OUTPUT_MASK, &output_mask);
    if (result < 0) {
        printf("INFO: QXT_GET_OUTPUT_MASK not supported\n");
        printf("This is expected - many IOCTL commands require specific driver support\n\n");
    } else {
        printf("SUCCESS: Got output mask\n");
        print_binary(output_mask, "Outputs");
        printf("\n");
    }

    // Try to set an output bit
    printf("Test 2: Attempting to set output bit 0...\n");
    result = ioctl(fd, QXT_SET_OUTPUT_BIT, 0);
    if (result < 0) {
        printf("INFO: QXT_SET_OUTPUT_BIT not supported (error: %s)\n", strerror(errno));
        printf("Note: Output control may require different IOCTL command codes\n\n");
    } else {
        printf("SUCCESS: Output bit 0 set\n\n");
        sleep(1);

        printf("Test 3: Clearing output bit 0...\n");
        result = ioctl(fd, QXT_CLEAR_OUTPUT_BIT, 0);
        if (result < 0) {
            printf("INFO: QXT_CLEAR_OUTPUT_BIT not supported\n\n");
        } else {
            printf("SUCCESS: Output bit 0 cleared\n\n");
        }
    }

    // Try to set full output mask
    printf("Test 4: Setting output mask pattern...\n");
    output_mask = 0x0000000F; // Set first 4 outputs
    result = ioctl(fd, QXT_SET_OUTPUT_MASK, &output_mask);
    if (result < 0) {
        printf("INFO: QXT_SET_OUTPUT_MASK not supported\n");
    } else {
        printf("SUCCESS: Set output mask to 0x%08x\n", output_mask);
        sleep(1);

        // Clear outputs
        output_mask = 0x00000000;
        ioctl(fd, QXT_SET_OUTPUT_MASK, &output_mask);
        printf("Cleared all outputs\n");
    }
    printf("\n");
}

// Get device version/info
void get_device_info(int fd) {
    printf("\n========================================\n");
    printf("DEVICE INFORMATION\n");
    printf("========================================\n\n");

    // Try version command
    uint32_t version = 0;
    int result = ioctl(fd, QXT_GET_VERSION, &version);
    if (result < 0) {
        printf("QXT_GET_VERSION: Not supported or wrong parameters\n");
    } else {
        printf("Device Version: 0x%08x\n", version);
        printf("  Major: %d\n", (version >> 24) & 0xFF);
        printf("  Minor: %d\n", (version >> 16) & 0xFF);
        printf("  Build: %d\n", version & 0xFFFF);
    }

    // Get file info
    int flags = fcntl(fd, F_GETFL);
    if (flags >= 0) {
        printf("\nFile Descriptor Info:\n");
        printf("  Flags: 0x%x ", flags);
        if (flags & O_RDWR) printf("(O_RDWR) ");
        if (flags & O_NONBLOCK) printf("(O_NONBLOCK) ");
        printf("\n");
    }

    // Check device size
    off_t size = lseek(fd, 0, SEEK_END);
    if (size >= 0) {
        printf("  Device size: %ld bytes\n", (long)size);
        lseek(fd, 0, SEEK_SET);
    } else {
        printf("  Device type: Stream (no seek support)\n");
    }

    printf("\n");
}

// Interactive menu
void show_menu() {
    printf("\n========================================\n");
    printf("QUIXANT CORE I/O CONTROL\n");
    printf("========================================\n");
    printf("1. Monitor inputs (continuous)\n");
    printf("2. Read current input state (single)\n");
    printf("3. Test output control\n");
    printf("4. Get device information\n");
    printf("5. Raw data dump (10 reads)\n");
    printf("6. Exit\n");
    printf("----------------------------------------\n");
    printf("Choice: ");
    fflush(stdout);
}

// Raw data dump
void raw_data_dump(int fd) {
    printf("\n========================================\n");
    printf("RAW DATA DUMP\n");
    printf("========================================\n\n");

    unsigned char buffer[256];

    for (int i = 0; i < 10 && keep_running; i++) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = read(fd, buffer, sizeof(buffer));

        if (bytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("Read %d: No data available\n", i + 1);
            } else {
                perror("read() error");
                break;
            }
        } else if (bytes == 0) {
            printf("Read %d: EOF\n", i + 1);
        } else {
            printf("Read %d: Got %zd bytes: ", i + 1, bytes);
            for (ssize_t j = 0; j < bytes && j < 32; j++) {
                printf("%02x ", buffer[j]);
            }
            if (bytes > 32) printf("...");
            printf("\n");
        }

        usleep(200000); // 200ms between reads
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int fd;
    int interactive = 1;

    // Check for command line options
    if (argc > 1) {
        if (strcmp(argv[1], "--monitor") == 0 || strcmp(argv[1], "-m") == 0) {
            interactive = 0;
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("Usage: %s [OPTIONS]\n\n", argv[0]);
            printf("Options:\n");
            printf("  -m, --monitor    Start in continuous monitoring mode\n");
            printf("  -h, --help       Show this help message\n");
            printf("  (no options)     Interactive menu mode\n\n");
            return 0;
        }
    }

    // Set up signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("========================================\n");
    printf("Quixant CORE I/O Example Program\n");
    printf("========================================\n");
    printf("Hardware: QXi-7000Lite\n");
    printf("Driver: qxtio v0.7.0.1\n");
    printf("Device: %s\n\n", DEVICE_PATH);

    // Open device
    printf("Opening device %s...\n", DEVICE_PATH);
    fd = open(DEVICE_PATH, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        perror("ERROR: Failed to open device");
        printf("Make sure:\n");
        printf("  1. The qxtio driver is loaded (lsmod | grep qxtio)\n");
        printf("  2. The device file exists (ls -l /dev/qxtio)\n");
        printf("  3. You have permission to access it\n");
        return 1;
    }
    printf("SUCCESS: Device opened (fd=%d)\n", fd);

    // Non-interactive mode: go straight to monitoring
    if (!interactive) {
        monitor_inputs(fd);
        close(fd);
        return 0;
    }

    // Interactive mode: show menu
    get_device_info(fd);

    while (keep_running) {
        show_menu();

        char input[16];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        int choice = atoi(input);

        switch (choice) {
            case 1:
                monitor_inputs(fd);
                break;
            case 2:
                {
                    uint32_t inputs = 0;
                    int result = ioctl(fd, QXT_GET_INPUT_MASK, &inputs);
                    if (result < 0) {
                        ssize_t bytes = read(fd, &inputs, sizeof(inputs));
                        if (bytes > 0) {
                            printf("\nCurrent input state (via read):\n");
                            print_binary(inputs, "Inputs");
                        } else {
                            printf("Unable to read input state\n");
                        }
                    } else {
                        printf("\nCurrent input state (via IOCTL):\n");
                        print_binary(inputs, "Inputs");
                    }
                }
                break;
            case 3:
                test_outputs(fd);
                break;
            case 4:
                get_device_info(fd);
                break;
            case 5:
                raw_data_dump(fd);
                break;
            case 6:
                printf("\nExiting...\n");
                keep_running = 0;
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
    }

    // Cleanup
    printf("\nClosing device...\n");
    close(fd);
    printf("Done.\n\n");

    return 0;
}
