#include "technician.h"

#include "raylib.h"

#include "stdio.h"

static Device* FindDeviceForAction(
	Device devices[],
	int deviceCount,
	int selectedDeviceId
)
{
	for (int i = 0; i < deviceCount; i++)
	{
		if (devices[i].id == selectedDeviceId)
		{
			return &devices[i];
		}
	}

	return NULL;
}

static float ClampActionValue(
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

void HandleTechnicianActions(
	Device devices[],
	int deviceCount,
	int selectedDeviceId,
	Alert alerts[],
	int* alertCount,
	float currentTime,
	char actionFeedbackMessage[],
	int actionFeedbackMessageLength,
	float* lastActionTime,
	ActivityLogEntry activityLog[],
	int* activityLogCount
)
{
	Device* selectedDevice =
		FindDeviceForAction(
			devices,
			deviceCount,
			selectedDeviceId
		);

	if (selectedDevice == NULL)
	{
		return;
	}

	const bool actionKeyPressed =
		IsKeyPressed(KEY_P) ||
		IsKeyPressed(KEY_D) ||
		IsKeyPressed(KEY_R) ||
		IsKeyPressed(KEY_E);
	
	const float actionCooldownSeconds = 1.25f;

	if (actionKeyPressed &&
		currentTime - *lastActionTime < actionCooldownSeconds)
	{
		snprintf(
			actionFeedbackMessage,
			actionFeedbackMessageLength, 
			"Action cooling down..."
		);

		return;
	}

	char message[MAX_ALERT_MESSAGE_LENGTH];

	if (IsKeyPressed(KEY_P))
	{
		snprintf(
			message,
			MAX_ALERT_MESSAGE_LENGTH,
			"%s ping completed",
			selectedDevice->name
		);

		RecordAlert(
			alerts,
			alertCount,
			ALERT_INFO,
			message,
			currentTime
		);

		snprintf(
			actionFeedbackMessage,
			actionFeedbackMessageLength,
			"Action completed: %s",
			message
		);

		*lastActionTime = currentTime;

		RecordActivityLogEntry(
			activityLog,
			activityLogCount,
			selectedDevice,
			"Ping",
			message,
			currentTime
		);
	}

	if (IsKeyPressed(KEY_D))
	{
		AlertSeverity severity = ALERT_INFO;

		if (selectedDevice->status == STATUS_CRITICAL ||
			selectedDevice->packetLossPercent > 5.0f)
		{
			severity = ALERT_CRITICAL;
		}
		else if (selectedDevice->status == STATUS_WARNING ||
			selectedDevice->latencyMilliseconds > 60.0f ||
			selectedDevice->healthPercent < 80.0f)
		{
			severity = ALERT_WARNING;
		}

		snprintf(
			message,
			MAX_ALERT_MESSAGE_LENGTH,
			"%s diagnostics completed",
			selectedDevice->name
		);

		RecordAlert(
			alerts,
			alertCount,
			severity,
			message,
			currentTime
		);

		snprintf(
			actionFeedbackMessage,
			actionFeedbackMessageLength,
			"Action completed: %s",
			message
		);

		*lastActionTime = currentTime;

		RecordActivityLogEntry(
			activityLog,
			activityLogCount,
			selectedDevice,
			"Diagnose",
			message,
			currentTime
		);
	}

	if (IsKeyPressed(KEY_R))
	{
		selectedDevice->healthPercent =
			ClampActionValue(
				selectedDevice->healthPercent + 12.0f,
				0.0f,
				100.0f
			);

		selectedDevice->latencyMilliseconds =
			ClampActionValue(
				selectedDevice->latencyMilliseconds - 12.0f,
				1.0f,
				85.0f
			);

		selectedDevice->packetLossPercent =
			ClampActionValue(
				selectedDevice->packetLossPercent - 2.0f,
				0.0f,
				10.0f
			);

		if (!selectedDevice->isInMaintenance &&
			selectedDevice->status != STATUS_MAINTENANCE &&
			selectedDevice->healthPercent >= 80.0f)
		{
			selectedDevice->status = STATUS_ONLINE;
		}

		snprintf(
			message,
			MAX_ALERT_MESSAGE_LENGTH,
			"%s restart completed",
			selectedDevice->name
		);

		RecordAlert(
			alerts,
			alertCount,
			ALERT_INFO,
			message,
			currentTime
		);

		snprintf(
			actionFeedbackMessage,
			actionFeedbackMessageLength,
			"Sction completed: %s",
			message
		);

		*lastActionTime = currentTime;

		RecordActivityLogEntry(
			activityLog,
			activityLogCount,
			selectedDevice,
			"Restart",
			message,
			currentTime
		);
	}

	if (IsKeyPressed(KEY_E))
	{
		snprintf(
			message,
			MAX_ALERT_MESSAGE_LENGTH,
			"%s escalated to network admin",
			selectedDevice->name
		);

		RecordAlert(
			alerts,
			alertCount,
			ALERT_CRITICAL,
			message,
			currentTime
		);

		snprintf(
			actionFeedbackMessage,
			actionFeedbackMessageLength,
			"Action completed %s",
			message
		);

		*lastActionTime = currentTime;

		RecordActivityLogEntry(
			activityLog,
			activityLogCount,
			selectedDevice,
			"Escalate",
			message,
			currentTime
		);
	}
}

void HandleTechnicianIncidentActions(
	Incident incidents[],
	int incidentCount,
	int selectedDeviceId,
	float currentTime,
	float lastActionTime
)
{
	if (selectedDeviceId == -1)
	{
		return;
	}

	const float actionCooldownSeconds = 1.25f;

	if (currentTime - lastActionTime > 0.05f)
	{
		return;
	}

	if (IsKeyPressed(KEY_D))
	{
		AcknowledgeIncidentForDevice(
			incidents,
			incidentCount,
			selectedDeviceId,
			currentTime
		);
	}

	if (IsKeyPressed(KEY_R))
	{
		ResolveIncidentForDevice(
			incidents,
			incidentCount,
			selectedDeviceId,
			currentTime
		);
	}

	if (IsKeyPressed(KEY_E))
	{
		EscalateIncidentForDevice(
			incidents,
			incidentCount,
			selectedDeviceId,
			currentTime
		);
	}
}


