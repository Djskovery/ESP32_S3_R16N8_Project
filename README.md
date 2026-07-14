# WiFi and MQTT Integration with Remote LED Control

Lab_6 demonstrates multi-task IoT connectivity on the ESP32 S3 DevKit R16N8 board, integrating WiFi connectivity, MQTT messaging, and remote device control through the CoreIoT platform. This project implements a distributed task-based architecture using FreeRTOS to manage concurrent network operations and hardware control.

**Key Features:**
  - **WiFi Connectivity Management:**
    + Station (STA) mode WiFi connection with automatic reconnection logic
    + Continuous monitoring of WiFi connection status every 5 seconds
    + Graceful reconnection handling when connection is lost
    + Serial debugging output showing connection state and IP address

  - **MQTT Communication with CoreIoT:**
    + MQTT client integration using PubSubClient library
    + Automatic MQTT broker reconnection with retry logic (2-second intervals)
    + Subscription to ThingsBoard/CoreIoT standard RPC request topics (v1/devices/me/rpc/request/+)
    + Token-based authentication using access token credentials
    + JSON payload parsing for RPC method calls

  - **Remote LED Control via RPC:**
    + LED control through MQTT RPC requests from CoreIoT platform
    + LED state toggled ON/OFF based on remote commands
    + RPC response publishing to confirm command execution
    + Eliminates "Request timeout" warnings on CoreIoT dashboard

  - **Multi-Task Architecture:**
    +  **WiFi Task** (Priority 2): Manages WiFi connection and reconnection at 5-second intervals
    + **MQTT Task** (Priority 1): Handles MQTT connection, message loop, and callback processing
    + **LED Task** (Priority 1): Receives commands via FreeRTOS queue and controls GPIO output
    + Queue-based inter-task communication for loose coupling and reliable message delivery

**Hardware Control:**

  - GPIO Pin 48: LED output control (HIGH = ON, LOW = OFF)
  - Asynchronous command handling through queue mechanism
  - Non-blocking LED state transitions

**Technology Stack:**
  - Framework: Arduino with FreeRTOS
  - Board: ESP32 S3 Gen4 R16N8
  - Build System: PlatformIO
  - Serial Communication: 115200 baud
  - Libraries: WiFi, PubSubClient, ArduinoJson
  - IoT Platform: CoreIoT (ThingsBoard-compatible)
  - Protocols: WiFi (802.11b/g/n), MQTT 3.1.1

**Architecture Highlights:**

This lab provides practical experience with production-grade IoT patterns including asynchronous task scheduling, network resilience, inter-process communication through queues, and remote device management through cloud platforms. The modular design separates concerns across dedicated tasks, enabling scalable addition of new features without disrupting existing functionality.
