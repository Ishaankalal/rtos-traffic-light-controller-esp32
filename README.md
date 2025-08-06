
# Smart RTOS-Based Traffic Light Controller 🚦 (ESP32 + FreeRTOS)

A real-time traffic light control system implemented using ESP32 and FreeRTOS. The system includes task scheduling, interrupt handling, button-based emergency override, hardware watchdog timer, and deterministic LED timing.

## 🔧 Tech Stack

- ESP32 wroom 32
- FreeRTOS (built-in with Arduino framework)
- Embedded C using Arduino IDE
- Serial Monitor for debugging

## 🚦 Features

- RTOS-based single task state machine using `vTaskDelayUntil()`
- Emergency override using push-button interrupt (`attachInterrupt()`)
- Inter-task notification with `vTaskNotifyGiveFromISR()` and `ulTaskNotifyTake()`
- Hardware Watchdog Timer using `esp_task_wdt` (10s timeout)
- GPIO control for RED, YELLOW, GREEN lights
- Debouncing for emergency button
- Serial output for status monitoring

## 🧠 Concepts Demonstrated

- RTOS task creation and synchronization
- ISR to task communication
- Real-time scheduling
- Watchdog-based fault detection
- Edge detection and debounce logic
- Cycle time control and delay calibration

## 📷 Hardware Setup

- **ESP32 Dev Board**
- **LEDs:** RED (GPIO 21), GREEN (GPIO 18), YELLOW (GPIO 19)
- **Button:** Emergency Trigger (GPIO 23)

Wiring Diagram (Optional: You can add a photo or Fritzing diagram here)

## 🛠️ How to Run

1. Open the code in Arduino IDE
2. Select board: `ESP32 Dev Module`
3. Install required ESP32 board package
4. Upload code to ESP32
5. Press the emergency button during GREEN phase to test ISR override

## 📁 Code Overview

- `main.ino`: Contains full RTOS logic with tasks, ISR, and watchdog setup

## 🎯 Applications

- Embedded traffic control systems
- Safety-critical RTOS applications
- ISR handling in real-time projects
- Entry-level IoT/embedded systems project for interviews

## 🧾 License

MIT License
