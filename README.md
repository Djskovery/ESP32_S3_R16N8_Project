# Radar Mini Project

Radar_Mini is a real-time miniature radar visualization system running on an ESP32-S3 DevKit (R16N8). The system combines an HC-SR04 ultrasonic distance sensor, a servo motor for scanning, and an OLED display for visual output. Remote control and telemetry are provided via MQTT (CoreIoT/ThingsBoard). The firmware uses FreeRTOS to separate sensing, control, and display tasks.

## Project Overview
- Real-time radar-like visualization on a 128×64 OLED.
- 180° sweeping using a servo; distance measurements taken per angle using HC-SR04.
- Remote control (start/stop scanning, commands) via MQTT RPC.
- Thread-safe access to shared peripherals (I2C) and multitasking via FreeRTOS.

## Key Features
- Live radar display on SH1106 / 128×64 OLED.
- Ultrasonic ranging with HC-SR04 (approx. up to 40 cm reliable).
- Servo-based 180° scanning with automatic and manual control modes.
- MQTT integration with CoreIoT / ThingsBoard for remote control and telemetry.
- FreeRTOS-based tasks to separate WiFi, MQTT, sensing, servo control, and display.
- WiFi STA mode for connecting to network and broker.

## Hardware Components

| Component          | Specification                | GPIO Pin            |
|-------------------:|-----------------------------:|--------------------:|
| ESP32-S3 DevKit     | R16N8 variant                | -                   |
| HC-SR04 Ultrasonic  | Distance up to ~40 cm        | TRIG: GPIO6, ECHO: GPIO5 |
| Servo Motor         | 180° rotation                | PWM: GPIO4          |
| SH1106 OLED Display | 128×64, I2C                  | SCL: GPIO9, SDA: GPIO8 |

## Software Architecture (FreeRTOS tasks)

The firmware is organized into multiple FreeRTOS tasks. Each source file implements one task with the responsibilities below.

- wifi_task.cpp
  - Connects to WiFi in STA (station) mode.
  - Monitors connection status and attempts auto-reconnect on disconnects.
  - Prepares network state needed by MQTT task (e.g., IP address).
  - Priority: 3

- mqtt_task.cpp
  - Connects to the CoreIoT / ThingsBoard MQTT broker.
  - Subscribes to RPC/command topics and handles incoming commands (e.g., start/stop scanning).
  - Publishes telemetry and status updates to the cloud.
  - Translates incoming MQTT commands into internal messages (pushes into xQueueServo).
  - Priority: 2

- sensor_task.cpp
  - Triggers the HC-SR04 TRIG pin and measures echo pulse duration.
  - Converts echo time into distance in cm and validates range.
  - Reads the current servo angle (shared/global or via safe access) and packages angle + distance into a Radar struct.
  - Sends Radar items to xQueueRadar for the display task to render.
  - Cycle: ~30 ms
  - Priority: 2

- servo_task.cpp
  - Drives the servo using PWM to set angles between 0° and 180°.
  - Implements automatic scanning (back-and-forth) when enabled.
  - Receives control commands from xQueueServo (ON/OFF or direct-angle commands) and updates scanning behavior.
  - Updates the shared angle variable so sensor_task measures at the correct angle.
  - Cycle: ~15 ms
  - Priority: 1

- oled_task.cpp
  - Renders the radar view (grid, angle marker, measured points, readouts) on the SH1106 OLED.
  - Consumes Radar structs from xQueueRadar and updates the display accordingly.
  - Protects I2C operations with xI2Cmutex to avoid concurrent access from other tasks.
  - Cycle: ~80 ms
  - Priority: 1

## Inter-Task Communication

- Queue: xQueueRadar
  - Purpose: transfer Radar data items (angle + distance) from sensor_task → oled_task.
  - Data type: Radar struct.

- Queue: xQueueServo
  - Purpose: send servo control commands (start/stop scanning, or set angle) from mqtt_task → servo_task.

- Mutex: xI2Cmutex
  - Purpose: protect shared I2C bus access for the OLED display when multiple tasks may access I2C.

## Data Structures

```cpp
typedef struct {
    int   angle;      // Current servo angle (0-180 °)
    float distance;   // Measured distance in cm; -1 indicates out-of-range/no echo
} Radar;
