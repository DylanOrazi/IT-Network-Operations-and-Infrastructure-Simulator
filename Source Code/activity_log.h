#ifndef ACTIVITY_LOG_H
#define ACTIVITY_LOG_H

#include "device.h"

#define MAX_ACTIVITY_LOG_ENTRIES 128
#define MAX_ACTIVITY_ACTION_LENGTH 64
#define MAX_ACTIVITY_RESULT_LENGTH 128

typedef struct ActivityLogEntry
{
	int id;

	float timestampSeconds;

	int deviceId;
	char deviceName[MAX_DEVICE_NAME_LENGTH];

	char action[MAX_ACTIVITY_ACTION_LENGTH];
	char result[MAX_ACTIVITY_RESULT_LENGTH];

	DeviceStatus deviceStatusAtAction;

	float healthPercentAtAction;
	float latencyMillisecondsAtAction;
	float packetLossPercentAtAction;
} ActivityLogEntry;

void InitializeActivityLog(
	ActivityLogEntry activityLog[],
	int* activityLogCount
);

void RecordActivityLog(
	ActivityLogEntry activityLog[],
	int* activityLogCount,
	const Device* device,
	const char* action,
	float timestampSeconds
);

int GetActivityLogCount(
	int activityLogCount
);

#endif


