#ifndef SENSORS_H
#define SENSORS_H

#include "system_types.h"

void updateMeasurements(SystemData &sys);
float readTemperatureC();
void measureFrequencyAndDuty(float &freqHz, float &dutyPercent);

#endif