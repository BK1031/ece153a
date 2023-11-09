<div style="text-align:right;">Bharat Kathi</div>
<div style="text-align:right;">ECE 153A</div>
<div style="text-align:right;">11/8/23</div>

# HW 4

## Q1

**a)**
<p align="middle">
  <img src="https://github.com/BK1031/ece153a/blob/main/hw4/a.png?raw=true" width="500" />
</p>

**b)** 

Input signals: Q_Entry_Sig, Q_Exit_Sig, Q_Init_Sig, Tick_Sig, F1_Sig, F2_Sig, F3_Sig, F4_Sig, F5_Sig, Terminate_Sig, Print_Sig.

State Vars: floor_req_curr[5], floor_pen[5], curr_dir, curr_floor, stop_time, move_time, floor_curr_call_time[5], floor_calls[5], floor_total_time[5]

**c)**

Q_TRAN() is used to trigger a state transition within the FSM, Q_SUPER() indicates that an event should be handled in the superstate of the current state. Q_HANDLED() indicates that the event has been successfully handled within the current state.

**d)**

When the elevator is stopped and a call for any floor is made, the following actions happen:
- Check if the floor is already pending with floor_pen[x]. If it is already pending no further action is taken and the event is ignored
- Else if the floor is NOT already pending, then we do the following:
    - Mark the floor as requested
    - Update the pending status for the floor (updatePending(x))
    - Record the time when the request was made
    - Increment the number of calls to that floor

**e)**

When the elevator is in a stopped state and recieves a TICK_SIG, it does the following:
- Check if the current floor is pending, if the floor is pending
- If the current floor is pending, the elevator continues to stay stopped and the elevator increments a timer that keeps track of how long the elevator has been stopped at the current floor
- If the stopping time exceeds the threshold, the following actions are taken:
    - The stopping time counter is reset to zero.
    - The floor pending status is cleared
    - The code checks if there are any other pending floors
- If there are any other pending floors, the elevator switches to the moving state.

Conditions to switch into a moving state:
- Current floor is pending
- Stopping time exceeds threshold
- There are other floors pending.

**f)**

The actions taken on TICK_SIG when the elevator is moving is:
- Check if the elevator has reached the destination floor, elevator calculates the time spent in motion and checks if it has reached the time threshold for moving between floors
- If the time threshold is not reached, the elevator continues moving, and the elevator continues to increment a timer to measure time spent moving.
- If time threshold is reached:
    - Move time counter is reset to zero
    - Current floor is updated to the next floor based on the elevators direction of travel
- Check if there are other floors pending, if there are other floors pending, the elevator continues to move.

Conditions for switching to the stopped state:
- The evelatror reaches its destination floor
- There are no other floors pending, meaning the current floor is the only one pending.

## Q2

When R=200:
1) F1 Average time is 10 seconds
2) F2 Average time is 7 seconds
3) F3 Average time is 6 seconds
4) F4 Average time is 7 seconds
5) F5 Average time is 10 seconds

When R=100:
1) F1 Average time is 10 seconds
2) F2 Average time is 7 seconds
3) F3 Average time is 6 seconds
4) F4 Average time is 7 seconds
5) F5 Average time is 10 seconds

When R=50:
1) F1 Average time is 10 seconds
2) F2 Average time is 7 seconds
3) F3 Average time is 6 seconds
4) F4 Average time is 7 seconds
5) F5 Average time is 10 seconds

When R=20:
1) F1 Average time is 15 seconds
2) F2 Average time is 11 seconds
3) F3 Average time is 10 seconds
4) F4 Average time is 11 seconds
5) F5 Average time is 15 seconds.

When R=10:
1) F1 Average time is 43 seconds
2) F2 Average time is 27 seconds
3) F3 Average time is 22 seconds
4) F4 Average time is 27 seconds
5) F5 Average time is 43 seconds.

## Q3

When R=200:
1) F1 Average time is 10 seconds
2) F2 Average time is 7 seconds
3) F3 Average time is 6 seconds
4) F4 Average time is 7 seconds
5) F5 Average time is 10 seconds

When R=100:
1) F1 Average time is 10 seconds
2) F2 Average time is 7 seconds
3) F3 Average time is 6 seconds
4) F4 Average time is 7 seconds
5) F5 Average time is 10 seconds

When R=50:
1) F1 Average time is 10 seconds
2) F2 Average time is 7 seconds
3) F3 Average time is 6 seconds
4) F4 Average time is 7 seconds
5) F5 Average time is 10 seconds

When R=20:
1) F1 Average time is 11 seconds
2) F2 Average time is 7 seconds
3) F3 Average time is 6 seconds
4) F4 Average time is 7 seconds
5) F5 Average time is 11 seconds.

When R=10:
1) F1 Average time is 16 seconds
2) F2 Average time is 11 seconds
3) F3 Average time is 10 seconds
4) F4 Average time is 1 1seconds
5) F5 Average time is 16 seconds.

## Q4

For 1000 emergency calls, the emergency average time was 9.75 seconds.

## Q5

This code exhibits several instances of poorly designed hierarchy. One such example is the unnecessary signal handling redundancy present for every floor call, whether the elevator is stationary or in motion. To rectify this issue, we can streamline the code by establishing a unified handler for floor call signals, merging all relevant states into a single cohesive state. Furthermore, the same code is redundantly replicated for each floor. Instead of this repetition, we can encapsulate common operations within a reusable function, eliminating the need to duplicate the code. Additionally, the code does not effectively implement the hierarchical state machine concept it professes to follow. It lacks a clear distinction between superstates and normal states. A more effective approach would involve defining superstates to encapsulate related functionality and delegating specific signal handling to smaller substates.