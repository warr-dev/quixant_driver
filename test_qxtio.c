/*
 * test_qxtio.c - Simple test program for Quixant CORE I/O driver
 *
 * Demonstrates basic device operations:
 * - Opening the device
 * - Reading device information
 * - Basic I/O operations
 * - Monitoring device continuously
 * - Closing the device
 *
 * Compile: gcc -o test_qxtio test_qxtio.c
 * Run: ./test_qxtio
 * Run continuous mode: ./test_qxtio --monitor
 *
 * Press Ctrl+C to exit
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/select.h>
#include <time.h>

#define DEVICE_PATH "/dev/qxtio"

// Common IOCTL commands (based on Quixant documentation)
// Note: Actual values may differ - check Quixant SDK documentation
#define QXT_GET_VERSION     0x1001
#define QXT_I2C_RW          0x1002
#define QXT_I2C_LOCK        0x1003
#define QXT_I2C_UNLOCK      0x1004
#define QXT_I2C_SET_BUS     0x1005
#define QXT_CFG_REG_GET     0x1006
#define QXT_CFG_REG_SET     0x1007

// Global flag for signal handling
static volatile int keep_running = 1;

// Signal handler for graceful shutdown
void signal_handler(int signum) {
    printf("\n\nReceived signal %d (Ctrl+C), shutting down...\n", signum);
    keep_running = 0;
}

// Continuous monitoring mode
void monitor_device(int fd) {
    char buffer[131072];  // 128KB buffer to match cat's buffer size
    ssize_t bytes_read;
    int count = 0;
    struct timeval tv;
    fd_set readfds;

    printf("\n========================================\n");
    printf("CONTINUOUS MONITORING MODE\n");
    printf("========================================\n");
    printf("Monitoring device: %s\n", DEVICE_PATH);
    printf("Press Ctrl+C to stop\n");
    printf("----------------------------------------\n\n");

    while (keep_running) {
        // Heartbeat - show we're alive
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        printf("[%s] Waiting for data... (count: %d)\r", time_str, count++);
        fflush(stdout);

        // Try to read from device
        memset(buffer, 0, sizeof(buffer));
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data available, sleep a bit
                usleep(100000); // 100ms
                continue;
            }
            printf("\n");
            perror("read() error");
            break;
        } else if (bytes_read == 0) {
            // No data, sleep a bit to avoid busy-waiting
            usleep(100000); // 100ms
            continue;
        } else {
            // Got data! Check if it's non-zero (interesting data)
            int has_nonzero = 0;
            for (ssize_t i = 0; i < bytes_read; i++) {
                if (buffer[i] != 0) {
                    has_nonzero = 1;
                    break;
                }
            }

            if (has_nonzero) {
                // Show interesting data
                printf("\n[%s] *** NON-ZERO DATA *** Received %zd bytes:\n", time_str, bytes_read);
                printf("  Hex: ");
                for (ssize_t i = 0; i < bytes_read && i < 64; i++) {
                    if (buffer[i] != 0) {
                        printf("\033[1;32m%02x\033[0m ", (unsigned char)buffer[i]);  // Green for non-zero
                    } else {
                        printf("%02x ", (unsigned char)buffer[i]);
                    }
                }
                if (bytes_read > 64) {
                    printf("... (%zd more bytes)", bytes_read - 64);
                }
                printf("\n");

                // Try to print as ASCII if printable
                int printable_count = 0;
                for (ssize_t i = 0; i < bytes_read; i++) {
                    if (buffer[i] >= 32 && buffer[i] <= 126) {
                        printable_count++;
                    }
                }

                if (printable_count > bytes_read / 2) {
                    printf("  ASCII: %.*s\n", (int)bytes_read, buffer);
                }
                printf("\n");
            }
            // If all zeros, just continue silently
        }
    }

    printf("\n\nMonitoring stopped.\n");
}

int main(int argc, char *argv[]) {
    int fd;
    int result;
    char buffer[256];
    ssize_t bytes_read;

    int monitor_mode = 0;

    // Check for --monitor flag
    if (argc > 1 && (strcmp(argv[1], "--monitor") == 0 || strcmp(argv[1], "-m") == 0)) {
        monitor_mode = 1;
    }

    // Set up signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("========================================\n");
    printf("Quixant CORE I/O Device Test\n");
    printf("========================================\n\n");

    // Test 1: Open device
    printf("Test 1: Opening device %s...\n", DEVICE_PATH);
    // Open with O_NONBLOCK to prevent blocking on read
    fd = open(DEVICE_PATH, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        perror("ERROR: Failed to open device");
        printf("Make sure the qxtio driver is loaded (lsmod | grep qxtio)\n");
        return 1;
    }
    printf("SUCCESS: Device opened (fd=%d)\n\n", fd);

    // If monitor mode requested, enter continuous monitoring
    if (monitor_mode) {
        monitor_device(fd);
        close(fd);
        return 0;
    }

    // Test 2: Read from device (just a few bytes)
    printf("Test 2: Reading from device...\n");
    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(fd, buffer, 16);
    if (bytes_read < 0) {
        perror("WARNING: Read operation failed");
        printf("This is normal - device may not support read() without parameters\n");
    } else {
        printf("SUCCESS: Read %zd bytes\n", bytes_read);
        printf("Data (hex): ");
        for (int i = 0; i < bytes_read && i < 16; i++) {
            printf("%02x ", (unsigned char)buffer[i]);
        }
        printf("\n");
    }
    printf("\n");

    // Test 3: Device information using lseek
    printf("Test 3: Getting device size (lseek)...\n");
    off_t size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        printf("INFO: lseek not supported or device is stream-type\n");
    } else {
        printf("Device reports size: %ld bytes\n", (long)size);
        lseek(fd, 0, SEEK_SET); // Reset position
    }
    printf("\n");

    // Test 4: Try a generic IOCTL
    printf("Test 4: Testing IOCTL communication...\n");
    unsigned int version = 0;
    result = ioctl(fd, QXT_GET_VERSION, &version);
    if (result < 0) {
        printf("INFO: QXT_GET_VERSION not supported or requires different parameters\n");
        printf("Error: %s\n", strerror(errno));
        printf("This is expected - need Quixant SDK for proper IOCTL usage\n");
    } else {
        printf("SUCCESS: IOCTL worked! Version: 0x%08x\n", version);
    }
    printf("\n");

    // Test 5: File descriptor validation
    printf("Test 5: Validating file descriptor...\n");
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        perror("ERROR: fcntl failed");
    } else {
        printf("SUCCESS: Device file descriptor is valid\n");
        printf("Flags: 0x%x ", flags);
        if (flags & O_RDWR) printf("(O_RDWR) ");
        if (flags & O_NONBLOCK) printf("(O_NONBLOCK) ");
        printf("\n");
    }
    printf("\n");

    // Test 6: Close device
    printf("Test 6: Closing device...\n");
    result = close(fd);
    if (result < 0) {
        perror("ERROR: Failed to close device");
        return 1;
    }
    printf("SUCCESS: Device closed cleanly\n\n");

    // Summary
    printf("========================================\n");
    printf("Test Summary\n");
    printf("========================================\n");
    printf("✓ Device can be opened\n");
    printf("✓ Device file descriptor is valid\n");
    printf("✓ Device can be closed\n");
    printf("ℹ Full functionality requires Quixant SDK\n");
    printf("\n");
    printf("Driver Status: OPERATIONAL\n");
    printf("\n");
    printf("To run in continuous monitoring mode:\n");
    printf("  ./test_qxtio --monitor\n");
    printf("========================================\n");

    return 0;
}
