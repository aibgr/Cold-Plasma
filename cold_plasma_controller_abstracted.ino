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
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ======================================================
// OLED CONFIG
// ======================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ======================================================
// PIN DEFINITIONS
// ======================================================
// Adjust these pins according to your wiring
const uint8_t PIN_FREQ_INPUT   = 2;   // external signal input for frequency/duty measurement
const uint8_t PIN_ROCKER_SW    = 3;   // rocker switch / phase command input
const uint8_t PIN_FAN_PWM      = 5;   // PWM output for fan control
const uint8_t PIN_BUZZER       = 6;   // buzzer output
const uint8_t PIN_ALARM_LED    = 7;   // warning LED
const uint8_t PIN_RELAY_NC     = 8;   // normally-closed relay control
const uint8_t PIN_TEMP_SENSOR  = A0;  // temperature sensor analog input

// ======================================================
// SYSTEM STATES
// ======================================================
enum SystemState
{
  STATE_ADJUSTING = 0,
  STATE_RUNNING,
  STATE_OVERHEAT,
  STATE_RESTART_WAIT
};

SystemState currentState = STATE_ADJUSTING;

// ======================================================
// TIMING VARIABLES
// ======================================================
unsigned long lastDisplayUpdateMs = 0;
unsigned long lastMeasureUpdateMs = 0;
unsigned long overheatEnteredMs   = 0;

unsigned long phaseStartMicros    = 0;   // stopwatch base
bool stopwatchEnabled             = false;

// ======================================================
// MEASURED / DISPLAYED VARIABLES
// ======================================================
float measuredFrequencyHz = 0.0f;
float measuredDutyCycle   = 0.0f;
float measuredTempC       = 0.0f;

uint8_t fanDuty           = 0;     // 0..255
bool relayEnergized       = false;
bool alarmActive          = false;

// ======================================================
// THRESHOLDS (PUBLIC DEMO VALUES)
// Replace with validated project-specific values in private firmware
// ======================================================
const float TEMP_WARNING_C   = 55.0f;
const float TEMP_OVERHEAT_C  = 65.0f;
const float TEMP_RESTART_C   = 45.0f;

// ======================================================
// FORWARD DECLARATIONS
// ======================================================
void showStartupScreen();
void updateMeasurements();
void updateStateMachine();
void updateOutputs();
void updateDisplay();

float readTemperatureC();
void measureFrequencyAndDuty(float &freqHz, float &dutyPercent);
uint8_t computeFanPwmFromTemp(float tempC);

void setAlarm(bool enabled);
void setRelay(bool enabled);
const char* stateToText(SystemState s);
unsigned long getStopwatchSeconds();

// ======================================================
// SETUP
// ======================================================
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

  // NC relay assumed active by control logic; adapt to your driver stage
  setRelay(true);

  analogWrite(PIN_FAN_PWM, 0);

  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    // If OLED init fails, continue without display
    while (true)
    {
      digitalWrite(PIN_ALARM_LED, !digitalRead(PIN_ALARM_LED));
      delay(200);
    }
  }

  display.clearDisplay();
  display.display();

  showStartupScreen();

  lastDisplayUpdateMs = millis();
  lastMeasureUpdateMs = millis();
}

// ======================================================
// MAIN LOOP
// ======================================================
void loop()
{
  // Update measurements periodically
  if (millis() - lastMeasureUpdateMs >= 200)
  {
    lastMeasureUpdateMs = millis();
    updateMeasurements();
    updateStateMachine();
    updateOutputs();
  }

  // Refresh OLED every 1 second as requested
  if (millis() - lastDisplayUpdateMs >= 1000)
  {
    lastDisplayUpdateMs = millis();
    updateDisplay();
  }
}

// ======================================================
// STARTUP SCREEN
// ======================================================
void showStartupScreen()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 20);
  display.println(F("Cold Plasma"));
  display.display();
  delay(1600);
}

// ======================================================
// MEASUREMENT LAYER
// ======================================================
void updateMeasurements()
{
  measuredTempC = readTemperatureC();
  measureFrequencyAndDuty(measuredFrequencyHz, measuredDutyCycle);
}

float readTemperatureC()
{
  // Generic placeholder conversion
  // Replace with your real sensor model (NTC / LM35 / amplifier / calibrated table)
  int raw = analogRead(PIN_TEMP_SENSOR);
  float voltage = (raw / 1023.0f) * 5.0f;

  // Public demo mapping only
  float tempC = voltage * 20.0f; // example placeholder
  return tempC;
}

void measureFrequencyAndDuty(float &freqHz, float &dutyPercent)
{
  // Public demo implementation
  // For a more accurate production design, use interrupts/input capture/timer methods.

  unsigned long highTime = pulseIn(PIN_FREQ_INPUT, HIGH, 30000UL);
  unsigned long lowTime  = pulseIn(PIN_FREQ_INPUT, LOW, 30000UL);

  if (highTime == 0 || lowTime == 0)
  {
    freqHz = 0.0f;
    dutyPercent = 0.0f;
    return;
  }

  unsigned long period = highTime + lowTime;
  freqHz = 1000000.0f / (float)period;
  dutyPercent = (100.0f * (float)highTime) / (float)period;
}

// ======================================================
// STATE MACHINE
// ======================================================
void updateStateMachine()
{
  bool rockerRunCommand = (digitalRead(PIN_ROCKER_SW) == LOW);

  switch (currentState)
  {
    case STATE_ADJUSTING:
    {
      stopwatchEnabled = false;

      // move to RUNNING when rocker requests run and system is safe
      if (rockerRunCommand && measuredTempC < TEMP_OVERHEAT_C)
      {
        currentState = STATE_RUNNING;
        phaseStartMicros = micros();
        stopwatchEnabled = true;
      }
      break;
    }

    case STATE_RUNNING:
    {
      // back to adjust if rocker released
      if (!rockerRunCommand)
      {
        currentState = STATE_ADJUSTING;
        stopwatchEnabled = false;
      }
      // go to overheat if temperature exceeds limit
      else if (measuredTempC >= TEMP_OVERHEAT_C)
      {
        currentState = STATE_OVERHEAT;
        stopwatchEnabled = false;
        overheatEnteredMs = millis();
      }
      break;
    }

    case STATE_OVERHEAT:
    {
      // stay here until system cools down
      if (measuredTempC <= TEMP_RESTART_C)
      {
        currentState = STATE_RESTART_WAIT;
      }
      break;
    }

    case STATE_RESTART_WAIT:
    {
      // wait for user to return to adjusting phase
      if (!rockerRunCommand)
      {
        currentState = STATE_ADJUSTING;
      }
      break;
    }

    default:
      currentState = STATE_ADJUSTING;
      break;
  }
}

// ======================================================
// OUTPUT CONTROL
// ======================================================
void updateOutputs()
{
  // Fan control based on thermal condition
  fanDuty = computeFanPwmFromTemp(measuredTempC);

  // Overheat phase forces relay action
  if (currentState == STATE_OVERHEAT)
  {
    setRelay(false); // deactivate output path / safe state
  }
  else
  {
    setRelay(true);
  }

  // Alarm behavior
  bool warning = (measuredTempC >= TEMP_WARNING_C);
  bool overheat = (currentState == STATE_OVERHEAT);

  if (warning || overheat)
  {
    setAlarm(true);
  }
  else
  {
    setAlarm(false);
  }

  // In overheat, keep fan active strongly until cooldown
  if (currentState == STATE_OVERHEAT)
  {
    analogWrite(PIN_FAN_PWM, 255);
  }
  else
  {
    analogWrite(PIN_FAN_PWM, fanDuty);
  }
}

uint8_t computeFanPwmFromTemp(float tempC)
{
  if (tempC < 35.0f) return 0;
  if (tempC < 40.0f) return 70;
  if (tempC < 45.0f) return 100;
  if (tempC < 50.0f) return 140;
  if (tempC < 55.0f) return 180;
  if (tempC < 60.0f) return 220;
  return 255;
}

void setAlarm(bool enabled)
{
  alarmActive = enabled;

  if (!enabled)
  {
    digitalWrite(PIN_ALARM_LED, LOW);
    noTone(PIN_BUZZER);
    return;
  }

  // Simple public demo alarm behavior
  digitalWrite(PIN_ALARM_LED, (millis() / 250) % 2);
  tone(PIN_BUZZER, 2400);
}

void setRelay(bool enabled)
{
  relayEnergized = enabled;

  // Adapt this to your real relay driver polarity
  digitalWrite(PIN_RELAY_NC, enabled ? HIGH : LOW);
}

// ======================================================
// DISPLAY LAYER
// ======================================================
void updateDisplay()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Header / State
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("State: "));
  display.println(stateToText(currentState));

  // Frequency
  display.setCursor(0, 12);
  display.print(F("F: "));
  display.print(measuredFrequencyHz, 1);
  display.println(F(" Hz"));

  // Duty
  display.setCursor(0, 22);
  display.print(F("D: "));
  display.print(measuredDutyCycle, 1);
  display.println(F(" %"));

  // Temperature
  display.setCursor(0, 32);
  display.print(F("T: "));
  display.print(measuredTempC, 1);
  display.println(F(" C"));

  // Fan PWM
  display.setCursor(0, 42);
  display.print(F("Fan PWM: "));
  display.print(fanDuty);

  // Relay
  display.setCursor(0, 52);
  display.print(F("Relay: "));
  display.print(relayEnergized ? F("ON") : F("OFF"));

  // Stopwatch in RUNNING phase
  if (currentState == STATE_RUNNING)
  {
    display.setCursor(78, 12);
    display.print(F("t:"));
    display.print(getStopwatchSeconds());
    display.print(F("s"));
  }

  display.display();
}

const char* stateToText(SystemState s)
{
  switch (s)
  {
    case STATE_ADJUSTING:   return "ADJUST";
    case STATE_RUNNING:     return "RUN";
    case STATE_OVERHEAT:    return "OVERHEAT";
    case STATE_RESTART_WAIT:return "RESTART";
    default:                return "UNKNOWN";
  }
}

unsigned long getStopwatchSeconds()
{
  if (!stopwatchEnabled) return 0UL;
  return (micros() - phaseStartMicros) / 1000000UL;
}