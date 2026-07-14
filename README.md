# Software Timer Management

Lab_5 demonstrates FreeRTOS software timer functionality on the ESP32 S3 DevKit R16N8 board. This project creates and manages multiple software timers with independent callbacks and timer IDs.

**Key Features:**

  - **Two Concurrent Software Timers:**
    + **Timer 1:** Executes every 2000ms with a shared callback function, demonstrating periodic timer operation
    + **Timer 2:** Executes every 3000ms with the same callback function, showing how multiple timers can use a single callback handler

  - **Timer Identification and Management:**
    + Timers are identified using timer IDs managed through pvTimerGetTimerID() and vTimerSetTimerID()
    + Each timer maintains its own tick count via the timer ID, allowing independent tracking of execution cycles
    + Timer 1 stops after 10 executions; Timer 2 stops after 5 executions, demonstrating conditional timer termination

  - **Shared Callback Pattern:**
    + Single callback function handles events from multiple timers
    + Uses timer handle comparison to determine which timer triggered the callback
    + Serial output distinguishes between timers ("ahihi" for Timer 1, "ihaha" for Timer 2)

  - **Software-based Timing:**
    + FreeRTOS software timers operate independently of hardware timers
    + Uses xTimerCreate() with periodic mode (pdTRUE) for continuous execution
    + Implements automatic timer cleanup via xTimerStop() after reaching target execution count

**Technology Stack:**
  - Framework: Arduino with FreeRTOS
  - Board: ESP32 S3 Gen4 R16N8
  - Build System: PlatformIO
  - Serial Communication: 115200 baud for debug output

This lab provides practical experience with FreeRTOS software timers, callback mechanisms, and timer management on embedded systems.
