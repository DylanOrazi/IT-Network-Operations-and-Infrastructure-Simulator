#ifndef SIMULATION_H
#define SIMULATION_H

#include "device.h"

void UpdateDeviceMetrics(
	Device devices[],
	int deviceCount,
	float deltaTime
);

#endif
