/*
 * test_qxtio_buttons.c - Button/Input event monitor for Quixant CORE I/O
 *
 * This program attempts multiple methods to detect button press events:
 * 1. IOCTL polling for button states
 * 2. Read with different patterns
 * 3. Memory-mapped I/O detection
 *
 * Compile: gcc -o test_qxtio_buttons test_qxtio_buttons.c
 * Run: sudo ./test_qxtio_buttons
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <stdint.h>

#define DEVICE_PATH "/dev/qxtio"

// IOCTL commands based on Quixant documentation
#define QXT_GET_VERSION     0x1001
#define QXT_I2C_RW          0x1002
#define QXT_I2C_LOCK        0x1003
#define QXT_I2C_UNLOCK      0x1004
#define QXT_I2C_SET_BUS     0x1005
#define QXT_CFG_REG_GET     0x1006
#define QXT_CFG_REG_SET     0x1007

// Possible button/input related IOCTLs (guessing common patterns)
#define QXT_GET_INPUT_STATE 0x1010
#define QXT_GET_BUTTON_STATE 0x1011
#define QXT_GET_DIN_STATE   0x1012  // Digital Input
#define QXT_GET_LP_STATE    0x1013  // Logic Processor state

// Global flag for signal handling
static volatile int keep_running = 1;

// Signal handler
void signal_handler(int signum) {
    printf("\n\nReceived signal %d, shutting down...\n", signum);
    keep_running = 0;
}

// Try to read button state using various IOCTL commands
void try_ioctl_methods(int fd) {
    uint32_t value = 0;
    int result;

    printf("\n=== Testing IOCTL Methods ===\n");

    // Try various IOCTL commands that might return button states
    uint32_t ioctls[] = {
        QXT_GET_VERSION,
        QXT_GET_INPUT_STATE,
        QXT_GET_BUTTON_STATE,
        QXT_GET_DIN_STATE,
        QXT_GET_LP_STATE,
        QXT_CFG_REG_GET,
        0x2000, 0x2001, 0x2002,  // Try some other common ranges
        0x3000, 0x3001,
    };

    for (int i = 0; i < sizeof(ioctls)/sizeof(ioctls[0]); i++) {
        value = 0;
        result = ioctl(fd, ioctls[i], &value);
        if (result >= 0) {
            printf("  IOCTL 0x%04x: SUCCESS - returned 0x%08x (%u)\n",
                   ioctls[i], value, value);
        }
    }
    printf("\n");
}

// Monitor by reading small chunks and looking for changes
void monitor_with_small_reads(int fd) {
    unsigned char prev_buffer[256];
    unsigned char buffer[256];
    ssize_t bytes_read;
    int count = 0;
    int changes_detected = 0;

    printf("\n=== Small Read Method (256 bytes) ===\n");
    printf("Monitoring for data changes...\n");
    printf("Press buttons now!\n\n");

    // Initial read
    memset(prev_buffer, 0, sizeof(prev_buffer));
    lseek(fd, 0, SEEK_SET);
    read(fd, prev_buffer, sizeof(prev_buffer));

    while (keep_running && count < 100) {  // Run for 100 iterations (~10 seconds)
        usleep(100000);  // 100ms delay

        memset(buffer, 0, sizeof(buffer));
        lseek(fd, 0, SEEK_SET);
        bytes_read = read(fd, buffer, sizeof(buffer));

        if (bytes_read > 0) {
            // Compare with previous
            int changed = 0;
            for (int i = 0; i < bytes_read && i < sizeof(prev_buffer); i++) {
                if (buffer[i] != prev_buffer[i]) {
                    changed = 1;
                    printf("[%03d] CHANGE at byte %d: 0x%02x -> 0x%02x\n",
                           count, i, prev_buffer[i], buffer[i]);
                    changes_detected++;
                }
            }

            if (changed) {
                printf("      Context: ");
                for (int i = 0; i < 32 && i < bytes_read; i++) {
                    printf("%02x ", buffer[i]);
                }
                printf("\n");
            }

            memcpy(prev_buffer, buffer, sizeof(prev_buffer));
        }

        count++;
        if (count % 10 == 0) {
            printf(".");
            fflush(stdout);
        }
    }

    printf("\n\nTotal changes detected: %d\n", changes_detected);
}

// Monitor by reading at different offsets
void monitor_with_offsets(int fd) {
    unsigned char buffer[64];
    off_t offsets[] = {0, 64, 128, 256, 512, 1024, 4096};

    printf("\n=== Reading at Different Offsets ===\n");

    for (int i = 0; i < sizeof(offsets)/sizeof(offsets[0]); i++) {
        off_t pos = lseek(fd, offsets[i], SEEK_SET);
        if (pos < 0) {
            printf("Offset %ld: lseek failed\n", (long)offsets[i]);
            continue;
        }

        ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            printf("Offset %6ld (%d bytes): ", (long)offsets[i], (int)bytes_read);

            int has_nonzero = 0;
            for (int j = 0; j < bytes_read && j < 16; j++) {
                printf("%02x ", buffer[j]);
                if (buffer[j] != 0) has_nonzero = 1;
            }
            if (bytes_read > 16) printf("...");
            if (has_nonzero) printf(" [HAS DATA]");
            printf("\n");
        }
    }
}

// Monitor by looking for specific data patterns
void monitor_streaming_with_pattern_detection(int fd) {
    unsigned char buffer[8192];
    ssize_t bytes_read;
    int count = 0;
    uint32_t prev_patterns[16] = {0};

    printf("\n=== Streaming Pattern Detection ===\n");
    printf("Looking for repeating patterns that change...\n");
    printf("Press buttons now!\n\n");

    while (keep_running && count < 50) {  // Run for ~5 seconds
        usleep(100000);  // 100ms

        lseek(fd, 0, SEEK_SET);
        bytes_read = read(fd, buffer, sizeof(buffer));

        if (bytes_read >= 64) {
            // Look for 32-bit patterns at regular intervals
            for (int i = 0; i < 16 && (i * 32 + 4) <= bytes_read; i++) {
                uint32_t pattern = *(uint32_t*)(&buffer[i * 32]);

                if (pattern != prev_patterns[i] && pattern != 0) {
                    time_t now = time(NULL);
                    struct tm *tm_info = localtime(&now);
                    char time_str[20];
                    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

                    printf("[%s] Pattern[%02d] @ offset %3d: 0x%08x -> 0x%08x",
                           time_str, i, i * 32, prev_patterns[i], pattern);

                    // Show context
                    printf("  [");
                    for (int j = 0; j < 8 && (i * 32 + j) < bytes_read; j++) {
                        printf("%02x ", buffer[i * 32 + j]);
                    }
                    printf("]\n");

                    prev_patterns[i] = pattern;
                }
            }
        }

        count++;
        if (count % 10 == 0) {
            printf(".");
            fflush(stdout);
        }
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    int fd;

    // Set up signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("========================================\n");
    printf("Quixant Button/Input Event Monitor\n");
    printf("========================================\n\n");

    // Open device
    printf("Opening device %s...\n", DEVICE_PATH);
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("ERROR: Failed to open device");
        printf("Make sure:\n");
        printf("  1. The qxtio driver is loaded (lsmod | grep qxtio)\n");
        printf("  2. You have permissions (try with sudo)\n");
        return 1;
    }
    printf("SUCCESS: Device opened (fd=%d)\n", fd);

    // Method 1: Try various IOCTL commands
    try_ioctl_methods(fd);

    if (!keep_running) goto cleanup;

    // Method 2: Read at different offsets
    monitor_with_offsets(fd);

    if (!keep_running) goto cleanup;

    // Method 3: Monitor with small reads
    monitor_with_small_reads(fd);

    if (!keep_running) goto cleanup;

    // Method 4: Pattern detection
    monitor_streaming_with_pattern_detection(fd);

cleanup:
    printf("\n========================================\n");
    printf("Summary\n");
    printf("========================================\n");
    printf("If no button events were detected, the buttons might:\n");
    printf("  1. Require the Quixant SDK library to access\n");
    printf("  2. Be exposed through a different device node\n");
    printf("  3. Need specific IOCTL commands not tried here\n");
    printf("  4. Be accessible via /dev/input/event* devices\n");
    printf("\nCheck Quixant documentation or SDK for proper API.\n");
    printf("========================================\n");

    close(fd);
    return 0;
}
