#ifndef CONNECTION_H
#define CONNECTION_H

#include "raylib.h"

typedef enum ConnectionStatus
{
	CONNECTION_HEALTHY = 0,
	CONNECTION_DEGRADED,
	CONNECTION_DOWN,
	CONNECTION_MAINTENANCE
} ConnectionStatus;

typedef struct Connection
{
	int id;

	int sourceDeviceId;
	int destinationDeviceId;

	ConnectionStatus status;

	float bandwidthMegabits;
	float currentTrafficMegabits;
	float latencyMilliseconds;
	float packetLossPercent;

	bool isActive;
} Connection;

Connection CreateConnection(
	int id,
	int sourceDeviceId,
	int destinationDeviceId,
	ConnectionStatus status,
	float bandwidthMegabits
);

const char* GetConnectionStatusName(ConnectionStatus status);
Color GetConnectionStatusColor(ConnectionStatus status);

#endif 