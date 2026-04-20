#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ---------------- OLED ----------------
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define OLED_ADDR     0x3C

// ---------------- Pins ----------------
const uint8_t PIN_FREQ_INPUT  = 2;
const uint8_t PIN_ROCKER_SW   = 3;
const uint8_t PIN_FAN_PWM     = 5;
const uint8_t PIN_BUZZER      = 6;
const uint8_t PIN_ALARM_LED   = 7;
const uint8_t PIN_RELAY_NC    = 8;
const uint8_t PIN_TEMP_SENSOR = A0;

// Relay logic level
const uint8_t RELAY_ON_LEVEL  = HIGH;
const uint8_t RELAY_OFF_LEVEL = LOW;

// ---------------- Timing ----------------
const unsigned long DISPLAY_INTERVAL_MS = 1000UL;
const unsigned long MEASURE_INTERVAL_MS = 200UL;

// ---------------- Thermal thresholds ----------------
// Public demo values only
const float TEMP_WARNING_C  = 55.0f;
const float TEMP_OVERHEAT_C = 65.0f;
const float TEMP_RESTART_C  = 45.0f;

// ---------------- Fan mapping ----------------
// Public demo mapping only
const float FAN_TEMP_MIN_C = 35.0f;
const float FAN_TEMP_MAX_C = 60.0f;

#endif