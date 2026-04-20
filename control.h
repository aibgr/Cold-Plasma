#ifndef CONTROL_H
#define CONTROL_H

#include "system_types.h"

void initSystemData(SystemData &sys);
void updateStateMachine(SystemData &sys);
void updateOutputs(SystemData &sys);

uint8_t computeFanPwmFromTemp(float tempC);
unsigned long getStopwatchSeconds(const SystemData &sys);
const char* stateToText(SystemState state);

#endif