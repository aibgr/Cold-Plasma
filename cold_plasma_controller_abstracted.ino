/*
  Cold Plasma Controller - Public Demo Firmware | A. Bagheri
  Target: Arduino Nano v3 (ATmega328P)

  Purpose:
  - Demonstrates the overall structure of the embedded control firmware
  - Shows system phases: ADJUSTING, RUNNING, OVERHEAT, RESTART_WAIT
  - Displays frequency, duty cycle, temperature, relay state, and timer on OLED
  - Controls fan speed with PWM
  - Handles alarm LED + buzzer
  - Protects system with an overheat state machine

  Note:
  This public version is intentionally simplified for portfolio / repository presentation.
  Hardware-specific calibration values, final protection thresholds, and proprietary
  control details are omitted or abstracted.

*/
#include <Arduino.h>
#include "config.h"
#include "system_types.h"
#include "sensors.h"
#include "control.h"
#include "display_ui.h"

SystemData gSystem;

void setup()
{
  pinMode(PIN_FREQ_INPUT, INPUT);
  pinMode(PIN_ROCKER_SW, INPUT_PULLUP);
  pinMode(PIN_FAN_PWM, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_ALARM_LED, OUTPUT);
  pinMode(PIN_RELAY_NC, OUTPUT);
  pinMode(PIN_TEMP_SENSOR, INPUT);

  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_ALARM_LED, LOW);
  digitalWrite(PIN_RELAY_NC, RELAY_ON_LEVEL);
  analogWrite(PIN_FAN_PWM, 0);

  initDisplay();
  showStartupScreen();

  initSystemData(gSystem);
}

void loop()
{
  unsigned long nowMs = millis();

  if (nowMs - gSystem.lastMeasureUpdateMs >= MEASURE_INTERVAL_MS)
  {
    gSystem.lastMeasureUpdateMs = nowMs;

    updateMeasurements(gSystem);
    updateStateMachine(gSystem);
    updateOutputs(gSystem);
  }

  if (nowMs - gSystem.lastDisplayUpdateMs >= DISPLAY_INTERVAL_MS)
  {
    gSystem.lastDisplayUpdateMs = nowMs;
    updateDisplay(gSystem);
  }
}