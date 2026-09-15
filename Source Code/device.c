#include "device.h"

#include <stdio.h>
#include <string.h>

Device CreateDevice(
	int id,
	const char* name,
	const char* ipAddress,
	DeviceType type,
	DeviceStatus status,
	Vector2 topologyPosition,
	int parentDeviceId
)
{
	Device device;

	memset(&device, 0, sizeof(Device));

	device.id = id;

	snprintf(
		device.name,
		MAX_DEVICE_NAME_LENGTH,
		"%s",
		name
	);

	snprintf(
		device.ipAddress,
		MAX_IP_ADDRESS_LENGTH,
		"%s",
		ipAddress
	);

	device.type = type;
	device.status = status;

	device.healthPercent = 100.0f;
	device.cpuUsagePercent = 0.0f;
	device.memoryUsagePercent = 0.0f;
	device.diskUsagePercent = 0.0f;
	device.temperatureCelsius = 0.0f;
	device.latencyMilliseconds = 0.0f;
	device.packetLossPercent = 0.0f;

	device.uptimeSeconds = 0.0;

	device.topologyPosition = topologyPosition;

	device.parentDeviceId = parentDeviceId;
	device.activeIncidentId = -1;

	device.isInMaintenance = false;

	return device;
}

const char* GetDeviceTypeName(DeviceType type)
{
    switch (type)
    {
    case DEVICE_FIREWALL:
        return "Firewall";

    case DEVICE_ROUTER:
        return "Router";

    case DEVICE_CORE_SWITCH:
        return "Core Switch";

    case DEVICE_ACCESS_SWITCH:
        return "Access Switch";

    case DEVICE_SERVER:
        return "Server";

    case DEVICE_BACKUP_SERVER:
        return "Backup Server";

    case DEVICE_ACCESS_POINT:
        return "Access Point";

    case DEVICE_WORKSTATION:
        return "Workstation";

    case DEVICE_LAPTOP:
        return "Laptop";

    case DEVICE_PRINTER:
        return "Printer";

    default:
        return "Unknown";
    }
}

const char* GetDeviceStatusName(DeviceStatus status)
{
    switch (status)
    {
    case STATUS_ONLINE:
        return "Online";

    case STATUS_WARNING:
        return "Warning";

    case STATUS_CRITICAL:
        return "Critical";

    case STATUS_OFFLINE:
        return "Offline";

    case STATUS_MAINTENANCE:
        return "Maintenance";

    case STATUS_ISOLATED:
        return "Isolated";

    default:
        return "Unknown";
    }
}

Color GetDeviceStatusColor(DeviceStatus status)
{
    switch (status)
    {
    case STATUS_ONLINE:
        return (Color) { 88, 232, 158, 255 };

    case STATUS_WARNING:
        return (Color) { 255, 193, 77, 255 };

    case STATUS_CRITICAL:
        return (Color) { 255, 82, 105, 255 };

    case STATUS_OFFLINE:
        return (Color) { 105, 118, 132, 255 };

    case STATUS_MAINTENANCE:
        return (Color) { 98, 180, 255, 255 };

    case STATUS_ISOLATED:
        return (Color) { 190, 112, 255, 255 };

    default:
        return GRAY;
    }
}
