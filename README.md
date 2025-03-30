# Multilevel Feedback Queue (MLFQ) Scheduling Algorithm

## Overview
This project implements a **Multilevel Feedback Queue (MLFQ) Scheduling Algorithm** using **multithreading and conditional variables** in C. The scheduler consists of **four queues**, each with a different scheduling policy:

1. *Q0 (Round Robin - 5ms time quantum)*
2. *Q1 (First Come, First Served - FCFS)*
3. *Q2 (Priority Scheduling - Priority = Inverse of CPU time needed)*
4. *Q3 (Shortest Job First - SJF)*

Each process is initially placed in **Q0** and executes for **5ms** before moving to the next lower queue. If a process remains unfinished after reaching **Q3**, it is moved back to **Q0**, ensuring continuous execution.

## Features
- *Multithreading*: Each queue runs in a separate thread.
- *Synchronization*: Uses **pthread mutexes** and **conditional variables** to synchronize queues.
- *Time-sharing*: Each process gets **5ms execution time** before being demoted, except when its remaining execution time is less than 5ms, in which case it only runs for the remaining time.
- *Process Execution Tracking*: Displays remaining time and turnaround time for each process.

## Compilation & Execution
# Compilation

Use **gcc** to compile the program:
    gcc mlfq.c -o mlfq 


# Running the Program
    ./mlfq


## Sample Input & Output
# Sample Input
    Enter number of processes: 5
    Enter each Process name (A,B,..like that) CPU Time
    A 300
    B 150
    C 250
    D 350
    E 450

# Sample Output
    Process A ran in Q0 . Remaining time is 295 
    Process B ran in Q0 . Remaining time is 145 
    Process C ran in Q0 . Remaining time is 245 
    Process D ran in Q0 . Remaining time is 345 
    Process E ran in Q0 . Remaining time is 445 
    Process A ran in Q1 . Remaining time is 290 
    Process B ran in Q1 . Remaining time is 140 
    Process C ran in Q1 . Remaining time is 240 
    Process D ran in Q1 . Remaining time is 340 
    Process E ran in Q1 . Remaining time is 440 
    Process B ran in Q2 . Remaining time is 135 
    Process C ran in Q2 . Remaining time is 235 
    Process A ran in Q2 . Remaining time is 285 
    ...
    ...
    ...

    Turnaround time of process A is 1290
    Turnaround time of process B is 730
    Turnaround time of process C is 1135
    Turnaround time of process D is 1395
    Turnaround time of process E is 1500
    Average turnaround time is 1210.00 ms

## Code Explanation
# **1. Data Structures**
- *`process` struct* stores process name, remaining execution time, and turnaround time.
- Four **queues (q0, q1, q2, q3)** manage processes at different scheduling levels.

# **2. Execution Logic**
- The `execute()` function runs a process for at most 5ms.
- Threads execute processes in their respective queues.
- **Condition variables** (`pthread_cond_t`) and **mutex locks** (`pthread_mutex_t`) synchronize execution.
- When a queue is empty, the next queue starts execution using `pthread_cond_signal()`.

# **3. Threads for Each Queue**
- `round_robin()` (Q0 - executes first, moves to Q1 if unfinished)
- `fcfs()`(Q1 - moves unfinished processes to Q2)
- `priority()` (Q2 - sorts by priority, moves unfinished processes to Q3)
- `sjf()` (Q3 - sorts by shortest job first, unfinished processes return to Q0)

# Synchronization
- **Mutex Locks**: Ensure thread-safe access to queues.
- **Conditional Variables**: Coordinate execution between queues.

## Author
Athira Antony

