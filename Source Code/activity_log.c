#include "activity_log.h"

#include <stdio.h>
#include <string.h>

static int nextActivityLogId = 1;

void InitializeActivityLog(
	ActivityLogEntry activityLog[],
	int* activityLogCount
)
{
	memset(
		activityLog,
		0,
		sizeof(ActivityLogEntry) * MAX_ACTIVITY_LOG_ENTRIES
	);

	*activityLogCount = 0;
	nextActivityLogId = 1;
}

void RecordActivityLogEntry(
	ActivityLogEntry activityLog[],
	int* activityLogCount,
	const Device* device,
	const char* action,
	const char* result,
	float timestampSeconds
)
{
	if (device == NULL)
	{
		return;
	}

	for (int i = MAX_ACTIVITY_LOG_ENTRIES - 1; i > 0; i--)
	{
		activityLog[i] = activityLog[i - 1];
	}

	activityLog[0].id = nextActivityLogId;
	activityLog[0].timestampSeconds = timestampSeconds;

	activityLog[0].deviceId = device->id;

	snprintf(
		activityLog[0].deviceName,
		MAX_DEVICE_NAME_LENGTH,
		"%s",
		device->name
	);

	snprintf(
		activityLog[0].action,
		MAX_ACTIVITY_ACTION_LENGTH,
		"%s",
		action
	);

	snprintf(
		activityLog[0].result,
		MAX_ACTIVITY_RESULT_LENGTH,
		"%s",
		result
	);

	activityLog[0].deviceStatusAtAction =
		device->status;

	activityLog[0].healthPercentAtAction =
		device->healthPercent;

	activityLog[0].latencyMillisecondsAtAction =
		device->latencyMilliseconds;

	activityLog[0].packetLossPercentAtAction =
		device->packetLossPercent;

	nextActivityLogId++;

	if (*activityLogCount < MAX_ACTIVITY_LOG_ENTRIES)
	{
		(*activityLogCount)++;
	}
}

int GetActivityLogCount(
	int activityLogCount
)
{
	return activityLogCount;
}

