
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"  // ✅ Hardware WDT header

#define RED_PIN     21
#define GREEN_PIN   18
#define YELLOW_PIN  19
#define BUTTON_PIN  23

TaskHandle_t trafficTaskHandle;
volatile bool emergency = false;

// ISR: Emergency Button Trigger
void IRAM_ATTR buttonISR() {
  emergency = true;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(trafficTaskHandle, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Helper: Turn off all LEDs
void turnAllOff() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
}

// Traffic Light Task
void vTrafficControlTask(void *pvParameters) {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t fullCycle = pdMS_TO_TICKS(12000); // Total 12s

  // ✅ Register this task to the hardware watchdog
  esp_task_wdt_add(NULL);

  while (1) {
    if (emergency) {
      turnAllOff();
      digitalWrite(RED_PIN, HIGH);
      Serial.println("🚨 EMERGENCY: RED ON");

      while (digitalRead(BUTTON_PIN) == LOW) vTaskDelay(50 / portTICK_PERIOD_MS);
      while (digitalRead(BUTTON_PIN) == HIGH) vTaskDelay(50 / portTICK_PERIOD_MS);
      while (digitalRead(BUTTON_PIN) == LOW) vTaskDelay(50 / portTICK_PERIOD_MS);

      digitalWrite(RED_PIN, LOW);
      emergency = false;
      ulTaskNotifyTake(pdTRUE, 0);
      xLastWakeTime = xTaskGetTickCount();
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }

    // Step 1: RED - 5s
    turnAllOff();
    digitalWrite(RED_PIN, HIGH);
    Serial.println("🔴 RED ON");
    for (int i = 0; i < 50; i++) {
      if (ulTaskNotifyTake(pdTRUE, 0)) break;
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    if (emergency) continue;

    // ✅ Feed watchdog after RED phase
    esp_task_wdt_reset();

    // Step 2: GREEN - 5s
    turnAllOff();
    digitalWrite(GREEN_PIN, HIGH);
    Serial.println("🟢 GREEN ON");
    for (int i = 0; i < 50; i++) {
      if (ulTaskNotifyTake(pdTRUE, 0)) break;
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    if (emergency) continue;

    // ✅ Feed watchdog after GREEN phase
    esp_task_wdt_reset();

    // Step 3: YELLOW - 2s
    turnAllOff();
    digitalWrite(YELLOW_PIN, HIGH);
    Serial.println("🟡 YELLOW ON");
    for (int i = 0; i < 20; i++) {
      if (ulTaskNotifyTake(pdTRUE, 0)) break;
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // ✅ Feed watchdog after YELLOW phase
    esp_task_wdt_reset();

    turnAllOff();

    // Final sync delay for precise 12s period
    vTaskDelayUntil(&xLastWakeTime, fullCycle);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button = active LOW
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  // ✅ Enable hardware watchdog with 10s timeout
  esp_task_wdt_init(10, true);

  // Create RTOS task
  xTaskCreate(vTrafficControlTask, "TrafficTask", 4096, NULL, 1, &trafficTaskHandle);
}

void loop() {
  // All logic handled by FreeRTOS task
}
