<div style="text-align:right;">Bharat Kathi</div>
<div style="text-align:right;">ECE 153A</div>
<div style="text-align:right;">10/25/23</div>

# HW 3

## Q1

State  | State'(0) | Out(0) | State'(1) | Out(1) |
---|------|------|------|------|
a|   b   |    1  |  c    |  0    |  
b|    f  |    0  |  d    |   0   |  
c|     a |    0  |   c   |   0   |  
d|   c   |    1  |   e   |   0   |  
e|   a   |    0  |    c  |   0   |  
f|   b   |    0  |   d   |   0   |  


We can see that C and E have the same transitions, we can consolidate them into one state.
Additionally, we can see that B and F have similar transitions, and would work if we
consolidated them into the same state.

State  | State'(0) | Out(0) | State'(1) | Out(1) |
-------|-----------|--------|-----------|--------|
a      | b / f     |    1   |  c / e    |  0     |  
b / f  |  b / f    |    0   |  d        |   0    |  
c / e  |     a     |    0   |   c / e   |   0    |  
d      |   c / e   |    1   |   c / e   |   0    |  

## Q2

**Parts A and B**

<p align="middle">
  <img src="https://github.com/BK1031/ece153a/blob/main/hw3/2.png?raw=true" width="500" />
</p>

Firstly, this state machine works for question A in that it accurately describes the behavior of the
quadrature encoder, only producing an output if the proper sequence of states are executed. Secondly, accounting for every combination of our two bit input for each state satisfies part B.

Let's say we start at the first start state (11), and then we transition to CW01 due to an input 01. While the first bit bounces between 0 and 1, we will keep oscillating between start and CW01 until the first bit finally settles back into CW01. Going from CW01 to CW00. While the second bit bounces, we will simply be going from state CW01 and CW00 until the second bit finally settles and we are in CW00. This logic holds true for every state transition we see here excep when transitioning from CCW01 to Start and CW10 to Start. While in either CW10 or CCW01, once the proper input bit is set once to be 11, we will go straight to Start, where it would then oscillate betwen CCW10 if we started from CW10, and CW01 if we start from CCW01. This is so that we dont produce multiple outputs as the bit oscillates between 11 and the previous state.