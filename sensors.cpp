#include <Arduino.h>
#include "config.h"
#include "sensors.h"

void updateMeasurements(SystemData &sys)
{
  sys.measuredTempC = readTemperatureC();
  measureFrequencyAndDuty(sys.measuredFrequencyHz, sys.measuredDutyCycle);
  sys.rockerRunCommand = (digitalRead(PIN_ROCKER_SW) == LOW);
}

float readTemperatureC()
{
  // Public demo placeholder conversion
  // Replace with real calibrated sensor logic in private firmware
  int raw = analogRead(PIN_TEMP_SENSOR);
  float voltage = (raw / 1023.0f) * 5.0f;
  float tempC = voltage * 20.0f;
  return tempC;
}

void measureFrequencyAndDuty(float &freqHz, float &dutyPercent)
{
  // Public demo method
  // For production, timer/interrupt-based measurement is recommended
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
  dutyPercent = 100.0f * ((float)highTime / (float)period);
}