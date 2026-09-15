#include "connection.h"

#include <string.h>

Connection CreateConnection(
	int id,
	int sourceDeviceId,
	int destinationDeviceId,
	ConnectionStatus status,
	float bandwidthMegabits
)
{
	Connection connection;

	memset(&connection, 0, sizeof(Connection));

	connection.id = id;

	connection.sourceDeviceId = sourceDeviceId;
	connection.destinationDeviceId = destinationDeviceId;

	connection.status = status;

	connection.bandwidthMegabits = bandwidthMegabits;
	connection.currentTrafficMegabits = 0.0f;
	connection.latencyMilliseconds = 0.0f;
	connection.packetLossPercent = 0.0f;

	connection.isActive =
		status != CONNECTION_DOWN;

	return connection;
}

const char* GetConnectionStatusName(ConnectionStatus status)
{
	switch (status)
	{
		case CONNECTION_HEALTHY:
			return "Healthy";

		case CONNECTION_DEGRADED:
			return "Degraded";

		case CONNECTION_DOWN:
			return "Down";

		case CONNECTION_MAINTENANCE:
			return "Maintenance";

		default:
			return "Unknown";
	}
}

Color GetConnectionStatusColor(ConnectionStatus status)
{
	switch (status)
	{
	case CONNECTION_HEALTHY:
		return (Color) { 88, 232, 158, 255 };

	case CONNECTION_DEGRADED:
		return (Color) { 255, 193, 77, 255 };

	case CONNECTION_DOWN:
		return (Color) { 255, 82, 105, 255 };

	case CONNECTION_MAINTENANCE:
		return (Color) { 98, 180, 255, 255 };

	default:
		return GRAY;
	}
}

	