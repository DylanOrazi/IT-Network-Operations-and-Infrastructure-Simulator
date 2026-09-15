#ifndef DEVICE_H
#define DEVICE_H

#include "raylib.h"

#define MAX_DEVICE_NAME_LENGTH 64
#define MAX_IP_ADDRESS_LENGTH 16

typedef enum DeviceType
{
	DEVICE_FIREWALL = 0,
	DEVICE_ROUTER,
	DEVICE_CORE_SWITCH,
	DEVICE_ACCESS_SWITCH,
	DEVICE_SERVER,
	DEVICE_BACKUP_SERVER,
	DEVICE_ACCESS_POINT,
	DEVICE_WORKSTATION,
	DEVICE_LAPTOP,
	DEVICE_PRINTER,
} DeviceType;

typedef enum DeviceStatus
{
	STATUS_ONLINE = 0,
	STATUS_WARNING,
	STATUS_CRITICAL,
	STATUS_OFFLINE,
	STATUS_MAINTENANCE,
	STATUS_ISOLATED
} DeviceStatus;

typedef struct Device
{
	int id;

	char name[MAX_DEVICE_NAME_LENGTH];
	char ipAddress[MAX_IP_ADDRESS_LENGTH];

	DeviceType type;
	DeviceStatus status;

	float healthPercent;
	float cpuUsagePercent;
	float memoryUsagePercent;
	float diskUsagePercent;
	float temperatureCelsius;
	float latencyMilliseconds;
	float packetLossPercent;

	double uptimeSeconds;

	Vector2 topologyPosition;

	int parentDeviceId;
	int activeIncidentId;

	bool isInMaintenance;
} Device;

Device CreateDevice(
	int id,
	const char* name,
	const char* ipAddress,
	DeviceType type,
	DeviceStatus status,
	Vector2 topologyPosition,
	int parentDeviceId
);

const char* GetDeviceTypeName(DeviceType type);
const char* GetDeviceStatusName(DeviceStatus status);
Color GetDeviceStatusColor(DeviceStatus status);

#endif 

