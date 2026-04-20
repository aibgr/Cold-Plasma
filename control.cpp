#include <Arduino.h>
#include "config.h"
#include "control.h"

static void setRelay(bool enabled);
static void setAlarm(bool enabled);

void initSystemData(SystemData &sys)
{
  sys.state = STATE_ADJUSTING;

  sys.measuredFrequencyHz = 0.0f;
  sys.measuredDutyCycle   = 0.0f;
  sys.measuredTempC       = 0.0f;

  sys.fanPwm         = 0;
  sys.relayEnabled   = true;
  sys.alarmEnabled   = false;
  sys.rockerRunCommand = false;

  sys.stopwatchEnabled = false;
  sys.phaseStartMicros = 0;
  sys.overheatEnteredMs = 0;

  sys.lastDisplayUpdateMs = millis();
  sys.lastMeasureUpdateMs = millis();
}

void updateStateMachine(SystemData &sys)
{
  switch (sys.state)
  {
    case STATE_ADJUSTING:
      sys.stopwatchEnabled = false;

      if (sys.rockerRunCommand && sys.measuredTempC < TEMP_OVERHEAT_C)
      {
        sys.state = STATE_RUNNING;
        sys.phaseStartMicros = micros();
        sys.stopwatchEnabled = true;
      }
      break;

    case STATE_RUNNING:
      if (!sys.rockerRunCommand)
      {
        sys.state = STATE_ADJUSTING;
        sys.stopwatchEnabled = false;
      }
      else if (sys.measuredTempC >= TEMP_OVERHEAT_C)
      {
        sys.state = STATE_OVERHEAT;
        sys.stopwatchEnabled = false;
        sys.overheatEnteredMs = millis();
      }
      break;

    case STATE_OVERHEAT:
      if (sys.measuredTempC <= TEMP_RESTART_C)
      {
        sys.state = STATE_RESTART_WAIT;
      }
      break;

    case STATE_RESTART_WAIT:
      if (!sys.rockerRunCommand)
      {
        sys.state = STATE_ADJUSTING;
      }
      break;

    default:
      sys.state = STATE_ADJUSTING;
      break;
  }
}

void updateOutputs(SystemData &sys)
{
  bool warning  = (sys.measuredTempC >= TEMP_WARNING_C);
  bool overheat = (sys.state == STATE_OVERHEAT);

  sys.fanPwm = computeFanPwmFromTemp(sys.measuredTempC);

  if (overheat)
  {
    sys.relayEnabled = false;
    sys.fanPwm = 255;
  }
  else
  {
    sys.relayEnabled = true;
  }

  sys.alarmEnabled = (warning || overheat);

  setRelay(sys.relayEnabled);
  setAlarm(sys.alarmEnabled);

  analogWrite(PIN_FAN_PWM, sys.fanPwm);
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

unsigned long getStopwatchSeconds(const SystemData &sys)
{
  if (!sys.stopwatchEnabled)
    return 0UL;

  return (micros() - sys.phaseStartMicros) / 1000000UL;
}

const char* stateToText(SystemState state)
{
  switch (state)
  {
    case STATE_ADJUSTING:   return "ADJUST";
    case STATE_RUNNING:     return "RUN";
    case STATE_OVERHEAT:    return "OVERHEAT";
    case STATE_RESTART_WAIT:return "RESTART";
    default:                return "UNKNOWN";
  }
}

static void setRelay(bool enabled)
{
  digitalWrite(PIN_RELAY_NC, enabled ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
}

static void setAlarm(bool enabled)
{
  if (!enabled)
  {
    digitalWrite(PIN_ALARM_LED, LOW);
    noTone(PIN_BUZZER);
    return;
  }

  digitalWrite(PIN_ALARM_LED, (millis() / 250UL) % 2UL);
  tone(PIN_BUZZER, 2400);
}