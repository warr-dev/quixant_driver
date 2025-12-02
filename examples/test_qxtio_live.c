/*
 * test_qxtio_live.c - Live streaming monitor for Quixant button events
 *
 * This monitors the actual streaming data from the device and highlights
 * any bytes that change, which should catch button press events.
 *
 * Compile: gcc -o test_qxtio_live test_qxtio_live.c
 * Run: ./test_qxtio_live
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define DEVICE_PATH "/dev/qxtio"
#define BUFFER_SIZE 131072

static volatile int keep_running = 1;

void signal_handler(int signum) {
    printf("\n\nShutting down...\n");
    keep_running = 0;
}

// Compare two buffers and show differences
void compare_and_display(unsigned char *prev, unsigned char *curr, size_t size, int iteration) {
    int changes = 0;
    int first_change = -1;

    for (size_t i = 0; i < size; i++) {
        if (prev[i] != curr[i]) {
            if (first_change == -1) first_change = i;
            changes++;
        }
    }

    if (changes > 0) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

        printf("\n╔═══════════════════════════════════════════════════════════╗\n");
        printf("║ [%s] CHANGE DETECTED! Iteration #%d\n", time_str, iteration);
        printf("╠═══════════════════════════════════════════════════════════╣\n");
        printf("║ Total bytes changed: %d\n", changes);
        printf("║ First change at offset: 0x%04x (%d)\n", first_change, first_change);
        printf("╠═══════════════════════════════════════════════════════════╣\n");

        // Show first few changes in detail
        int shown = 0;
        for (size_t i = 0; i < size && shown < 10; i++) {
            if (prev[i] != curr[i]) {
                printf("║ [0x%04lx] 0x%02x -> 0x%02x", i, prev[i], curr[i]);

                // Show bit changes
                unsigned char diff = prev[i] ^ curr[i];
                printf("  (bits: ");
                for (int bit = 7; bit >= 0; bit--) {
                    if (diff & (1 << bit)) {
                        printf("\033[1;33m%d\033[0m", (curr[i] >> bit) & 1);
                    } else {
                        printf("%d", (curr[i] >> bit) & 1);
                    }
                }
                printf(")\n");
                shown++;
            }
        }

        if (changes > shown) {
            printf("║ ... and %d more changes\n", changes - shown);
        }

        // Show context around first change
        if (first_change >= 0 && first_change < size) {
            printf("╠═══════════════════════════════════════════════════════════╣\n");
            printf("║ Context (16 bytes around first change):\n");
            printf("║ Previous: ");
            int start = (first_change - 8 > 0) ? first_change - 8 : 0;
            for (int i = start; i < start + 16 && i < size; i++) {
                if (i == first_change) printf("\033[1;31m");
                printf("%02x ", prev[i]);
                if (i == first_change) printf("\033[0m");
            }
            printf("\n║ Current:  ");
            for (int i = start; i < start + 16 && i < size; i++) {
                if (i == first_change) printf("\033[1;32m");
                printf("%02x ", curr[i]);
                if (i == first_change) printf("\033[0m");
            }
            printf("\n");
        }

        printf("╚═══════════════════════════════════════════════════════════╝\n");
    }
}

int main(int argc, char *argv[]) {
    int fd;
    unsigned char *buffer1, *buffer2;
    unsigned char *curr_buffer, *prev_buffer, *temp;
    ssize_t bytes_read;
    int iteration = 0;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Allocate buffers
    buffer1 = malloc(BUFFER_SIZE);
    buffer2 = malloc(BUFFER_SIZE);
    if (!buffer1 || !buffer2) {
        fprintf(stderr, "Failed to allocate buffers\n");
        return 1;
    }

    memset(buffer1, 0, BUFFER_SIZE);
    memset(buffer2, 0, BUFFER_SIZE);

    curr_buffer = buffer1;
    prev_buffer = buffer2;

    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║      Quixant Live Button/Input Event Monitor             ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");

    fd = open(DEVICE_PATH, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open device");
        free(buffer1);
        free(buffer2);
        return 1;
    }

    printf("✓ Device opened successfully\n");
    printf("✓ Monitoring for changes...\n");
    printf("✓ Press your buttons NOW!\n");
    printf("✓ Press Ctrl+C to exit\n\n");
    printf("Reading %d bytes per iteration...\n", BUFFER_SIZE);
    printf("─────────────────────────────────────────────────────────────\n");

    // Initial read
    bytes_read = read(fd, prev_buffer, BUFFER_SIZE);
    if (bytes_read > 0) {
        printf("Initial read: %zd bytes\n", bytes_read);
    }

    while (keep_running) {
        usleep(50000);  // 50ms between reads

        bytes_read = read(fd, curr_buffer, BUFFER_SIZE);

        if (bytes_read > 0) {
            iteration++;

            // Compare with previous read
            compare_and_display(prev_buffer, curr_buffer, bytes_read, iteration);

            // Swap buffers
            temp = prev_buffer;
            prev_buffer = curr_buffer;
            curr_buffer = temp;

            // Heartbeat every 20 iterations (~1 second)
            if (iteration % 20 == 0) {
                printf("\r[Monitoring... iteration %d]", iteration);
                fflush(stdout);
            }
        } else if (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("Read error");
            break;
        }
    }

    printf("\n\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║ Monitoring stopped after %d iterations\n", iteration);
    printf("╚═══════════════════════════════════════════════════════════╝\n");

    close(fd);
    free(buffer1);
    free(buffer2);

    return 0;
}
