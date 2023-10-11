<div style="text-align:right;">Bharat Kathi</div>
<div style="text-align:right;">ECE 153A</div>
<div style="text-align:right;">10/10/23</div>

# HW 1

## Q1

**a)** I set up a simple loop that runs 100k times. During each iteration, a random address is generated using `rand_int(CM_ADDRESS_SPACE_SIZE)`. Then the data is accessed using `cm_do_access(addr)` and the access cycles are added to a `access_time` variable outside the loop. When the loop finishes all its iterations, the `access_time` is divided by the number of iterations (100k in this case) to return the expected access time. This whole process was repeated again with the cache disabled.

The expected acess time when the cache is enabled was 18.44 cycles. When the cache is disabled, it was 12.0 cycles.

Here's my full code for this part:
```c
void part_a() {
    // Part A: Determine the expected access time with cache enabled and disabled, given random reads
    // of the 16-bit memory space.

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
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
        printf("Expected Access Time: %.2f cycles\n", (double)access_time / NUM_SIMULATIONS);
    }
}
```
Output:
```
Cache Enabled
Expected Access Time: 18.44 cycles
Cache Disabled
Expected Access Time: 12.00 cycles
```

**b)** For this part, we had to select the addresses based on certain probabilities. To do this I first initialized the `address` variable to 0. Then I setup a similar loop to before that runs 100k times. Then during each iteration I first generated a random integer between 0 and 99. 

If it was less than 60, then our next address should follow the current one. This was done using `address = (address + 1) % CM_ADDRESS_SPACE_SIZE;`. The last part was to account for rollover incase the current address was already the last possible one. 

If it was less that 95, then our next address should be within 40 words of the current one. The way I implemented this was by generating an offset using `int offset = rand_int(81) - 40;`. Adding this to the current `address` should allow us to get a new address within 40 words greater or less than (including an offset of 0) the current one.

If it was greater than or equal to 95, then we need to generate a random far address. This was done using `address = rand_int(CM_ADDRESS_SPACE_SIZE - 1);`.

Now that we generated the new address, we can follow the same steps as before to get the data, store the cycles, and return the expected access time. This whole process was repeated again with the cache disabled.

The expected acess time when the cache is enabled was 6.50 cycles. When the cache is disabled, it was 6.87 cycles.

Here's my full code for this part:
```c
void part_b() {
    // Part B: Assuming that:
    // s=0.6 is the probability that the next address follows the current one,
    // p=0.35 is the probability that the next address is not sequential but is within 40 words
    // (either direction) of the current one,
    // thus 0.05 is the probability of a random far address,
    // determine the expected values for access time with cache enabled and disabled.

    int sequential_prob = 60;
    int nearby_prob = 35;
    int far_prob = 5;

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
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
        printf("Expected Access Time: %.2f cycles\n", (double)access_time / NUM_SIMULATIONS);
    }
}
```
Output:
```
Cache Enabled
Expected Access Time: 6.50 cycles
Cache Disabled
Expected Access Time: 6.87 cycles
```

**c)** To find the worst case access time, I iterated through every single address in the address space and stored the longest access time.

The worst case access time was 12 cycles.

Here's my full code for this part:
```c
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
```
Output:
```
Worst Case Access Time: 12 cycles
```

**d)** I first modified the `NUM_SIMULATIONS` to be 1000 and then ran each previous part 24 times to get the data for the distributions. 



In a real system, this would be hard to estimate since there can be so many factors outside your actual program that can affect the process of accessing some memory.

Here's the raw data:
```
(a)
Cache Enabled: 18.62, 18.50, 18.64, 18.69, 18.59, 18.51, 18.46, 18.53, 18.62, 18.66, 18.77, 18.66, 18.64, 18.71, 18.62, 18.60, 18.71, 18.50, 18.59, 18.57, 18.59, 18.62, 18.62, 18.55
Cache Disabled: 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00, 12.00

(b)
Cache Enabled: 6.02, 6.49, 6.60, 6.58, 5.84, 6.53, 6.47, 6.67, 6.40, 6.54, 6.33, 6.78, 6.18, 6.54, 6.72, 5.82, 7.08, 6.65, 6.80, 6.17, 6.89, 6.63, 6.45, 6.65
Cache Disabled: 6.57, 6.85, 6.73, 6.73, 6.63, 7.12, 7.17, 6.97, 6.74, 6.95, 6.79, 6.67, 6.76, 6.75, 6.58, 7.05, 7.26, 6.86, 6.98, 6.78, 7.12, 6.62, 6.97, 6.98
```