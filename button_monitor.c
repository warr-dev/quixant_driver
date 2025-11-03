/*
 * button_monitor.c - Quixant Button Monitor using libqxt API
 *
 * This program uses the official Quixant libqxt library to read
 * Digital Input (DIN) states from the Quixant hardware.
 *
 * Compile: gcc -o button_monitor button_monitor.c -lqxt
 * Run: ./button_monitor
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <libqxt.h>

static volatile int keep_running = 1;

void signal_handler(int signum) {
    printf("\n\nShutting down...\n");
    keep_running = 0;
}

void print_binary(uint32_t value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");
    }
}

void print_input_changes(uint32_t prev, uint32_t curr, int num_inputs) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║ [%s] INPUT CHANGE DETECTED!\n", time_str);
    printf("╠════════════════════════════════════════════════════════════╣\n");

    for (int i = 0; i < num_inputs; i++) {
        int prev_state = (prev >> i) & 1;
        int curr_state = (curr >> i) & 1;

        if (prev_state != curr_state) {
            printf("║ DIN %2d: %d → %d  [%s]\n",
                   i,
                   prev_state,
                   curr_state,
                   curr_state ? "\033[1;32mPRESSED\033[0m" : "\033[1;31mRELEASED\033[0m");
        }
    }

    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ Previous: ");
    print_binary(prev, num_inputs);
    printf("\n║ Current:  ");
    print_binary(curr, num_inputs);
    printf("\n╚════════════════════════════════════════════════════════════╝\n");
}

int main(int argc, char *argv[]) {
    uint32_t prev_inputs = 0;
    uint32_t curr_inputs = 0;
    int iteration = 0;
    int num_inputs = 32;  // Quixant supports 32 DIN ports

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║      Quixant Button Monitor (using libqxt)                ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    // Initialize the Quixant device
    printf("Initializing Quixant device...\n");
    if (qxt_device_init() != 0) {
        fprintf(stderr, "ERROR: Failed to initialize Quixant device\n");
        fprintf(stderr, "Make sure:\n");
        fprintf(stderr, "  1. The qxtio driver is loaded (lsmod | grep qxtio)\n");
        fprintf(stderr, "  2. You have the proper permissions\n");
        fprintf(stderr, "  3. libqxt is installed\n");
        return 1;
    }
    printf("✓ Device initialized successfully\n\n");

    // Get hardware inventory info
    struct hw_inventory inventory;
    if (qxt_hw_inventory(&inventory) == 0) {
        printf("Hardware Information:\n");
        printf("  Driver Product: %s\n", inventory.driver_product);
        printf("  Driver Version: %s\n", inventory.driver_version);
        printf("  Library Product: %s\n", inventory.library_product);
        printf("  Library Version: %s\n", inventory.library_version);
        printf("  Log FW Version: %s\n", inventory.log_fw_version);
        printf("\n");
    }

    printf("✓ Monitoring %d digital inputs...\n", num_inputs);
    printf("✓ Press your buttons NOW!\n");
    printf("✓ Press Ctrl+C to exit\n\n");
    printf("────────────────────────────────────────────────────────────\n");

    // Initial read - note: inputs are inverted (0 = pressed)
    prev_inputs = ~qxt_dio_readdword(0);

    printf("Initial input state: ");
    print_binary(prev_inputs, num_inputs);
    printf("\n\n");

    while (keep_running) {
        usleep(50000);  // 50ms polling interval

        // Read current inputs (inverted: 0 = pressed, 1 = released)
        curr_inputs = ~qxt_dio_readdword(0);

        if (curr_inputs != prev_inputs) {
            print_input_changes(prev_inputs, curr_inputs, num_inputs);
            prev_inputs = curr_inputs;
        }

        iteration++;
        if (iteration % 20 == 0) {
            printf("\r[Monitoring... iteration %d]", iteration);
            fflush(stdout);
        }
    }

    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║ Monitoring stopped after %d iterations\n", iteration);
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return 0;
}
