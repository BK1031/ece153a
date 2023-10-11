#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "cache_model.h"

#define NUM_SIMULATIONS 1000

void part_a(bool cache_enabled) {
    // Part A: Determine the expected access time with cache enabled and disabled, given random reads
    // of the 16-bit memory space.
    if (cache_enabled) {
        cm_enable_cache();
        printf("Cache Enabled\n");
    } else {
        cm_disable_cache();
        printf("Cache Disabled\n");
    }
    int access_time = 0;
    for (int i = 0; i < NUM_SIMULATIONS; i++) {
        int addr = rand_int(CM_ADDRESS_SPACE_SIZE);
        cm_do_access(addr);
        access_time += cm_get_last_access_cycles();
    }
//    printf("%.2f, ", (double)access_time / NUM_SIMULATIONS);
    printf("Expected Access Time: %.2f cycles\n", (double)access_time / NUM_SIMULATIONS);
}

void part_b(bool cache_enabled) {
    // Part B: Assuming that:
    // s=0.6 is the probability that the next address follows the current one,
    // p=0.35 is the probability that the next address is not sequential but is within 40 words
    // (either direction) of the current one,
    // thus 0.05 is the probability of a random far address,
    // determine the expected values for access time with cache enabled and disabled.

    int sequential_prob = 60;
    int nearby_prob = 35;
    int far_prob = 5;

    if (cache_enabled) {
        cm_enable_cache();
        printf("Cache Enabled\n");
    } else {
        cm_disable_cache();
        printf("Cache Disabled\n");
    }

    int access_time = 0;
    int address = 0;
    for (int i = 0; i < NUM_SIMULATIONS; i++) {
        int prob = rand_int(100);
        if (prob < sequential_prob) {
            address = (address + 1) % CM_ADDRESS_SPACE_SIZE;
        } else if (prob < sequential_prob + nearby_prob) {
            int offset = rand_int(81) - 40;
            address = (address + offset + CM_ADDRESS_SPACE_SIZE) % CM_ADDRESS_SPACE_SIZE;
        } else {
            address = rand_int(CM_ADDRESS_SPACE_SIZE - 1);
        }
        cm_do_access(address);
        access_time += cm_get_last_access_cycles();
    }
//    printf("%.2f, ", (double)access_time / NUM_SIMULATIONS);
    printf("Expected Access Time: %.2f cycles\n", (double)access_time / NUM_SIMULATIONS);
}

void part_c() {
    // Part C: What is the worst case access time for the memory?

    int max_access_time = 0;
    for (int i = 0; i < CM_ADDRESS_SPACE_SIZE; i++) {
        cm_do_access(i);
        int access_time = cm_get_last_access_cycles();
        if (access_time > max_access_time)
            max_access_time = access_time;
    }

    printf("Worst Case Access Time: %d cycles\n", max_access_time);
}

void part_d() {
    printf("(a) Cache Enabled: ");
    for (int i = 0; i < 24; i++) {
        part_a(true);
    }
    printf("\n(a) Cache Disabled: ");
    for (int i = 0; i < 24; i++) {
        part_a(false);
    }
    printf("\n\n(b) Cache Enabled: ");
    for (int i = 0; i < 24; i++) {
        part_b(true);
    }
    printf("\n(b) Cache Disabled: ");
    for (int i = 0; i < 24; i++) {
        part_b(false);
    }
}

int main() {
    cm_init();

    // Generate a new random seed each time
    unsigned int seed = time(NULL);
    printf("random seed: %d", seed);
    srandom(seed);

    printf("\n==== PART A ====\n");
    part_a(true);
    part_a(false);
    printf("\n==== PART B ====\n");
    part_b(true);
    part_b(false);
    printf("\n==== PART C ====\n");
    part_c();
    printf("\n==== PART D ====\n");
//    part_d();

    return 0;
}