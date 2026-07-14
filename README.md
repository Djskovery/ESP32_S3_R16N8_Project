# FreeRTOS Queue Management Simulation

Lab_4 demonstrates FreeRTOS queue-based inter-task communication on the ESP32 S3. The project implements a multi-task system where requests are queued and processed by specialized handler tasks.

**Key Features:**
  - **Queue-Based Communication:** Uses FreeRTOS queues to pass data structures between tasks asynchronously
  - **Data Packet Structure:** Custom Data_t struct containing a target ID and data value
  - **Multiple Handler Tasks:**
    + **TaskReception:** Reads user input from Serial (format: [ID] [Value]) and enqueues requests
    + **TaskFunc1:** Handles requests with targetID == 1
    + **TaskFunc2:** Handles requests with targetID == 2
    + **TaskErrorHandle:** Catches and discards unhandled request IDs
  - **Priority-Based Scheduling:** Tasks run with different FreeRTOS priorities (3, 2, 2, 1)
  - **Peek Before Pop Pattern:** Handler tasks use xQueuePeek() to check if a request matches their ID before calling xQueueReceive() to consume it
  - **RTOS Concepts Demonstrated:** Task creation, queues, task delays, message passing, and error handling

**Technology Stack:**
  - **Framework:** Arduino with FreeRTOS
  - **Board:** ESP32 S3 Gen4 R16N8
  - **Build System:** PlatformIO

**Use Case:** Great for learning concurrent programming patterns, queue management, and real-time task scheduling on embedded systems.
