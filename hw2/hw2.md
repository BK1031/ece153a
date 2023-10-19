<div style="text-align:right;">Bharat Kathi</div>
<div style="text-align:right;">ECE 153A</div>
<div style="text-align:right;">10/18/23</div>

# HW 2

## Q1

**a)** Branch delay slot instructions allow the processor to keep working on instructions after a branch instruction is encountered and the processor doesn't know if it should take the branch or not. This keeps the pipeline of the processor filled with instructions so it does not delay/idle while waiting for the branch outcome.

**b)** When an interrupt occurs while the processor is in the middle of a hardware divide, it may not be able to respond in a timely manner due to having to wait for the hardware divide to complete. Additionally, hardware divides are relatively unpredictable in their execution times, which makes predicting the interrupt latency impossible.

**c)** 8B (parameters) + 8B (local variables) + 4B (return address) = 20B. Total calls = 4MB (stack size) / 20B (stack frame size) = 200,000 calls. If you go over it, you are likely to run into stack overflow, which will most likely crash the program.

**d)** Without using the word `static`, the storage for our array would be located in the stack. If we use the `static` keyword, it would be located in initialized memory.

**e)** Moving from BRAM to DRAM essentially changes where in the memory hierarchy our program exists, which essentially means we would be increasing the interrupt latency. Since the interrupt service routines are optimized for the BRAM latancies, the DRAM’s increased latency would explain why some interrupts are missing.

## Q2

Using the `volatile` keyword tells the compiler that it shouldn't optimize the code and it should continuously check `var` to ensure that it has not changed despite it looking like there is no way it could change.

## Q3

**a)** Interrupt 0 worst case was 34, interrupt 1 worst case was 9. Mean for interrupt 0 was 3.685 and the mean for interrupt 1 was 5.768. 95% confidence interval is 0.02196.

Here's the CDF graphs for interrupt 0 and 1.
<p align="middle">
  <img src="https://github.com/BK1031/ece153a/blob/main/hw2/interrupt0.png?raw=true" width="500" />
  <img src="https://github.com/BK1031/ece153a/blob/main/hw2/interrupt1.png?raw=true" width="500" />
</p>



**b)** Before doubling the probability of interrupt 0, we missed 7836 interrupt 0’s and 443 interrupt 1’s. After, we had 37173 missed interrupt 0’s and 645 missed interrupt ones, which is almost 5 times as many missed interrupt 0’s and almost 2x as many missed interrupt 1’s. The worst case latency for an interrupt 0 with the original probability was only 34 while with the new probability, it was 39. This makes sense considering we have twice as many interrupts to handle, so this overload of interrupts wouldve led to overall slowdown of the machine and an increased number of missed interrupts. The worst case latancy for interrupt 1 was not changed, at 9ms, which makes sense since interrupt 1 has more priority and there shouldnt be an increase in the number of interrupt 1’s that are being sent.