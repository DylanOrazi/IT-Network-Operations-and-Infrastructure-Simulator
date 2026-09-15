#include "simulation.h"

#include "raylib.h" 

static float ClampFloat(
	float value,
	float minimum,
	float maximum
)
{
	if (value < minimum)
	{
		return minimum;
	}

	if (value > maximum)
	{
		return maximum;
	}

	return value;
}

static float RandomFloatRange(
	float minimum,
	float maximum
)
{
	const float randomValue =
		(float)GetRandomValue(0, 10000) / 10000.0f;

	return minimum + randomValue * (maximum - minimum);
}

static void UpdateDeviceStatusFromHealth(Device* device)
{
	if (device->isInMaintenance ||
		device->status == STATUS_MAINTENANCE ||
		device->status == STATUS_ISOLATED) 
	{
		return;
	}

	if (device->healthPercent < 45.0f)
	{
		device->status = STATUS_CRITICAL;
	}
	else if (device->healthPercent < 80.0f)
	{
		device->status = STATUS_WARNING;
	}
	else
	{
		device->status = STATUS_ONLINE;
	}
}

static void UpdateOneDevice(
	Device* device,
	float deltaTime
)
{
	device->uptimeSeconds += deltaTime;

	if (device->isInMaintenance ||
		device->status == STATUS_MAINTENANCE)
	{
		device->cpuUsagePercent =
			ClampFloat(
				device->cpuUsagePercent + RandomFloatRange(-0.5f, 0.5f),
				0.0f,
				25.0f
			);

		device->memoryUsagePercent =
			ClampFloat(
				device->memoryUsagePercent + RandomFloatRange(-0.3f, 0.3f),
				0.0f,
				35.0f
			);

		device->latencyMilliseconds = 0.0f;
		device->packetLossPercent = 0.0f;

		return;
	}

	device->cpuUsagePercent =
		ClampFloat(
			device->cpuUsagePercent + RandomFloatRange(-3.5f, 3.5f),
			3.0f,
			98.0f
		);

	device->memoryUsagePercent =
		ClampFloat(
			device->memoryUsagePercent + RandomFloatRange(-2.0f, 2.0f),
			5.0f,
			96.0f
		);

	device->diskUsagePercent =
		ClampFloat(
			device->diskUsagePercent + RandomFloatRange(-0.4f, 0.8f),
			0.0f,
			99.0f
		);

	device->temperatureCelsius =
		ClampFloat(
			32.0f + (device->cpuUsagePercent * 0.42f) +
			RandomFloatRange(-2.0f, 2.0f),
			25.0f,
			92.0f
		);

	device->latencyMilliseconds =
		ClampFloat(
			device->latencyMilliseconds + RandomFloatRange(-1.5f, 1.5f),
			1.0f,
			85.0f
		);

	device->packetLossPercent =
		ClampFloat(
			device->packetLossPercent + RandomFloatRange(-0.3f, 0.3f),
			0.0f,
			10.0f
		);

	float healthScore = 100.0f;

	healthScore -= device->cpuUsagePercent * 0.12f;
	healthScore -= device->memoryUsagePercent * 0.10f;
	healthScore -= device->diskUsagePercent * 0.07f;
	healthScore -= device->packetLossPercent * 2.0f;

	if (device->latencyMilliseconds > 25.0f)
	{
		healthScore -=
			(device->latencyMilliseconds - 25.0f) * 0.35f;
	}

	if (device->temperatureCelsius > 70.0f)
	{
		healthScore -=
			(device->temperatureCelsius - 70.0f) * 1.2f; 
	}

	device->healthPercent =
		ClampFloat(
			healthScore,
			0.0f,
			100.0f
		);

	UpdateDeviceStatusFromHealth(device);
}

void UpdateDeviceMetrics(
	Device devices[],
	int deviceCount,
	float deltaTime
)
{
	static float updateTimer = 0.0f;
	updateTimer += deltaTime;

	if (updateTimer < 0.75f)
	{
		return;
	}

	updateTimer = 0.0f;

	for (int i = 0; i < deviceCount; i++)
	{
		UpdateOneDevice(
			&devices[i],
			deltaTime
		);
	}
}





