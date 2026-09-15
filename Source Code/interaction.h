#ifndef INTERACTION_H
#define INTERACTION_H

#include "raylib.h"
#include "device.h"

int GetClickedDeviceId(
	const Device devices[],
	int deviceCount
);

const Device* FindDeviceByIdForInteraction(
	const Device devices[],
	int deviceCount,
	int deviceId
);

void DrawSelectedDeviceHighlight(
	const Device* device
);

void DrawDeviceDetailsPanel(
	const Device* device
);

#endif 
