# Preemptive Time-Slice Task Scheduling

Lab_3 demonstrates a preemptive time-slice task scheduling simulation on the ESP32 S3 DevKit R16N8 board using FreeRTOS. This project creates and manages multiple concurrent tasks with different priorities and timing characteristics.

**Key Features:**
  - **Three Concurrent Tasks:**
    + **TaskA** (Priority 3 - Highest): Runs every 500ms with vTaskDelayUntil() for precise, periodic execution
    + **TaskB** (Priority 1): Runs with a 10ms delay, demonstrating lower-priority task behavior
    + **TaskC** (Priority 1): Runs with a 10ms delay, showing how multiple tasks with the same priority are scheduled

**Preemptive Scheduling:** The higher-priority TaskA preempts lower-priority tasks (TaskB and TaskC) when it needs to run

**Time-Slice Scheduling:** Lower-priority tasks share CPU time in a round-robin fashion

**Real-time Task Management:** Uses FreeRTOS kernel services for deterministic, real-time task scheduling

**Technology Stack:**
  - **Framework:** Arduino with FreeRTOS
  - **Board:** ESP32 S3 Gen4 R16N8
  - **Build System:** PlatformIO

This lab provides practical experience with real-time operating systems, task prioritization, and scheduling algorithms on embedded systems.
