#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

#include <Arduino.h>

enum SystemState
{
  STATE_ADJUSTING = 0,
  STATE_RUNNING,
  STATE_OVERHEAT,
  STATE_RESTART_WAIT
};

struct SystemData
{
  SystemState state;

  float measuredFrequencyHz;
  float measuredDutyCycle;
  float measuredTempC;

  uint8_t fanPwm;
  bool relayEnabled;
  bool alarmEnabled;
  bool rockerRunCommand;

  bool stopwatchEnabled;
  unsigned long phaseStartMicros;
  unsigned long overheatEnteredMs;

  unsigned long lastDisplayUpdateMs;
  unsigned long lastMeasureUpdateMs;
};

#endif