# Smart Green House
 
Smart Green House is an automated greenhouse monitoring and control system running on a **Yolo UNO (ESP32-S3)** board. The system continuously reads soil moisture, ambient temperature/humidity, and ambient light, displays live readings on an I2C LCD, and automatically drives a 4-channel relay module to water, ventilate, and light the plants. The firmware uses FreeRTOS to separate sensing, display, and actuation into independent tasks that share the I2C bus safely.

**Link**: [Video](https://drive.google.com/file/d/1N0z7yCzvDHBoBK55NSTUGl0t5AObLLIO/view?usp=sharing)
 
## Project Overview
- Real-time environmental monitoring: temperature, humidity, ambient light, and soil moisture.
- Live readings shown on a 16×2 I2C LCD.
- Fully automatic control of a water pump, a fan, and grow lights based on sensor thresholds.
- Thread-safe access to the shared I2C bus and multitasking via FreeRTOS.
## Key Features
- Temperature & humidity sensing with a DHT20 sensor.
- Ambient light sensing with a VEML6040 color sensor.
- Soil moisture sensing via a capacitive analog sensor, calibrated to a 0–100% scale.
- Live status display on a 16×2 I2C LCD.
- Automatic irrigation, ventilation, and grow-light control through a 4-channel I2C relay module.
- FreeRTOS-based tasks to separate sensing, display, and device control.
- Mutex-protected I2C bus shared safely across all peripherals.
## Hardware Components
 
| Component                         | Specification                     | Interface / Pin                     |
|-----------------------------------:|-----------------------------------:|-------------------------------------:|
| Yolo UNO                          | ESP32-S3, Arduino Uno form-factor | -                                    |
| DHT20 Temperature & Humidity Sensor | Digital, I2C                    | I2C4 connector (SDA: GPIO11, SCL: GPIO12) |
| VEML6040 Color / Light Sensor     | I2C                                | I2C2 connector (SDA: GPIO11, SCL: GPIO12) |
| 16×2 LCD Display                  | I2C                                | I2C3 connector (SDA: GPIO11, SCL: GPIO12) |
| 4-Channel Relay Module            | I2C                                | I2C1 connector (SDA: GPIO11, SCL: GPIO12) |
| Capacitive Soil Moisture Sensor   | Analog                             | A0 (GPIO1)                           |
 
**Relay channel mapping:**
 
| Relay Channel | Device                  |
|---------------:|-------------------------:|
| S1, S2         | Grow light               |
| S3             | Fan                      |
| S4             | Water pump / motor       |
 
## Software Architecture (FreeRTOS tasks)
 
The firmware is organized into multiple FreeRTOS tasks plus a shared header/source pair for global state.
 
- **global.h / global.cpp**
  - Central pin definitions (`SCL_I2C`, `SDA_I2C`, `MOISTURE_PIN`) and soil-moisture calibration constants (`MOISTURE_DRY`, `MOISTURE_WET`).
  - Defines the shared `SensorData` struct.
  - Declares the shared mutex (`xI2CMutex`) and queues (`xLcdQueue`, `xRelayQueue`) used across tasks.
- **main.cpp**
  - Entry point. Initializes `Serial` and the I2C bus (`Wire.begin`).
  - Creates the shared I2C mutex and both inter-task queues.
  - Spawns the three FreeRTOS tasks below.
- **sensors_task.h / sensors_task.cpp** (`SensorsTask`)
  - Reads temperature and humidity from the DHT20 sensor and ambient light from the VEML6040 sensor.
  - Reads the soil moisture sensor and maps the raw ADC value to a 0–100% scale using `MOISTURE_DRY` / `MOISTURE_WET`.
  - Packages the readings into a `SensorData` snapshot and publishes it to both `xLcdQueue` and `xRelayQueue`.
  - All I2C sensor reads are wrapped in `xI2CMutex`.
  - Cycle: ~3000 ms · Priority: 2
- **lcd_task.h / lcd_task.cpp** (`DisplayTask`)
  - Consumes `SensorData` items from `xLcdQueue`.
  - Renders temperature, light, humidity, and soil moisture on the 16×2 I2C LCD.
  - LCD writes are protected by `xI2CMutex`.
  - Priority: 1
- **relay_task.h / relay_task.cpp** (`ControlDevicesTask`)
  - Consumes `SensorData` items from `xRelayQueue`.
  - Drives the 4-channel relay module to automatically control the water pump (low soil moisture), fan (high temperature), and grow lights (low ambient light).
  - Relay commands are protected by `xI2CMutex`.
  - Priority: 1
## Automation Logic
 
| Condition                | Action                          |
|---------------------------|----------------------------------|
| Temperature ≥ 30 °C       | Turn fan **ON**, otherwise OFF   |
| Soil moisture < 80 %      | Turn water pump **ON**, otherwise OFF |
| Ambient light < threshold | Turn grow lights **ON**, otherwise OFF |
 
## Inter-Task Communication
 
- **Queue: `xLcdQueue`**
  - Purpose: transfer `SensorData` items from `SensorsTask` → `DisplayTask`.
  - Data type: `SensorData` struct.
- **Queue: `xRelayQueue`**
  - Purpose: transfer `SensorData` items from `SensorsTask` → `ControlDevicesTask`.
  - Data type: `SensorData` struct.
- **Mutex: `xI2CMutex`**
  - Purpose: protect the shared I2C bus, since the DHT20, VEML6040, LCD, and relay module all sit on the same SDA/SCL lines and are accessed from three different tasks.
## Data Structures
 
```cpp
typedef struct {
    float temp;      // Temperature in °C
    float humid;      // Relative humidity in %
    int   light;      // Ambient light reading (VEML6040 white channel)
    int   moisture;    // Soil moisture in % (0-100)
} SensorData;
```
