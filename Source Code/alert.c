#include "alert.h"

#include <stdio.h>
#include <string.h>

#define MAX_TRACKED_DEVICES 64

static DeviceStatus previousStatuses[MAX_TRACKED_DEVICES];
static float lastMetricAlertTime = 0.0f;
static bool hasStatusSnapshot = false;
static int nextAlertId = 1;

static const char* GetAlertSeverityName(AlertSeverity severity)
{
	switch (severity)
	{
		case ALERT_INFO:
			return "INFO";

		case ALERT_WARNING:
			return "WARNING";

		case ALERT_CRITICAL:
			return "CRITICAL";

		default:
			return "UNKNOWN";
	}
}

static Color GetAlertSeverityColor(AlertSeverity severity)
{
	switch (severity)
	{
		case ALERT_INFO:
			return (Color){ 105, 210, 255, 255 };

		case ALERT_WARNING:
			return (Color){ 255, 193, 77, 255 };

		case ALERT_CRITICAL:
			return (Color){ 255, 82, 105, 255 };

		default:
			return GRAY;
	}
}

static void AddAlert(
	Alert alerts[],
	int* alertCount,
	AlertSeverity severity,
	const char* message,
	float currentTime
)
{
	for (int i = MAX_ALERTS - 1; i > 0; i--)
	{
		alerts[i] = alerts[i - 1];
	}

	alerts[0].id = nextAlertId;
	alerts[0].severity = severity;
	alerts[0].timeCreated = currentTime;
	alerts[0].isActive = true;

	snprintf(
		alerts[0].message,
		MAX_ALERT_MESSAGE_LENGTH,
		"%s",
		message
	);

	nextAlertId++;

	if (*alertCount < MAX_ALERTS)
	{
		(*alertCount)++;
	}
}

void RecordAlert(
	Alert alerts[],
	int* alertCount,
	AlertSeverity severity,
	const char* message,
	float currentTime
)
{
	AddAlert(
		alerts,
		alertCount,
		severity,
		message,
		currentTime
	);
}

void InitializeAlertFeed(
	Alert alerts[],
	int* alertCount
)
{
	memset(
		alerts,
		0,
		sizeof(Alert) * MAX_ALERTS
	);

	*alertCount = 0;
	hasStatusSnapshot = false;
	lastMetricAlertTime = 0.0f;
	nextAlertId = 1;
}

void UpdateAlertFeedFromDevices(
	Alert alerts[],
	int* alertCount,
	const Device devices[],
	int deviceCount,
	float currentTime
)
{
	if (deviceCount > MAX_TRACKED_DEVICES)
	{
		deviceCount = MAX_TRACKED_DEVICES;
	}

	if (!hasStatusSnapshot)
	{
		for (int i = 0; i < deviceCount; i++)
		{
			previousStatuses[i] = devices[i].status;
		}

		hasStatusSnapshot = true;

		AddAlert(
			alerts,
			alertCount,
			ALERT_INFO,
			"Network monitoring started",
			currentTime
		);

		return;
	}

	for (int i = 0; i < deviceCount; i++)
	{
		if (devices[i].status != previousStatuses[i])
		{
			char message[MAX_ALERT_MESSAGE_LENGTH];

			snprintf(
				message,
				MAX_ALERT_MESSAGE_LENGTH,
				"%s changed to %s",
				devices[i].name,
				GetDeviceStatusName(devices[i].status)
			);

			AlertSeverity severity = ALERT_INFO;

			if (devices[i].status == STATUS_WARNING)
			{
				severity = ALERT_WARNING;
			}
			else if (devices[i].status == STATUS_CRITICAL ||
				devices[i].status == STATUS_OFFLINE)
			{
				severity = ALERT_CRITICAL;
			}

			AddAlert(
				alerts,
				alertCount,
				severity,
				message,
				currentTime
			);

			previousStatuses[i] = devices[i].status;
		}
	}

	if (currentTime - lastMetricAlertTime < 5.0f)
	{
		return;
	}

	lastMetricAlertTime = currentTime;

	for (int i = 0; i < deviceCount; i++)
	{
		char message[MAX_ALERT_MESSAGE_LENGTH];

		if (devices[i].healthPercent < 60.0f &&
			devices[i].status != STATUS_MAINTENANCE)
		{
			snprintf(
				message,
				MAX_ALERT_MESSAGE_LENGTH,
				"%s health below 60%%",
				devices[i].name
			);

			AddAlert(
				alerts,
				alertCount,
				ALERT_WARNING,
				message,
				currentTime
			);
		}

		if (devices[i].latencyMilliseconds > 60.0f &&
			devices[i].status != STATUS_MAINTENANCE)
		{
			snprintf(
				message,
				MAX_ALERT_MESSAGE_LENGTH,
				"%s latency exceeded 60 ms",
				devices[i].name
			);

			AddAlert(
				alerts,
				alertCount,
				ALERT_WARNING,
				message,
				currentTime
			);
		}

		if (devices[i].packetLossPercent > 5.0f &&
			devices[i].status != STATUS_MAINTENANCE)
		{
			snprintf(
				message,
				MAX_ALERT_MESSAGE_LENGTH,
				"%s packet loss exceeded 5%%",
				devices[i].name
			);

			AddAlert(
				alerts,
				alertCount,
				ALERT_CRITICAL,
				message,
				currentTime
			);
		}
	}
}

void DrawAlertFeed(
	const Alert alerts[],
	int alertCount,
	Rectangle bounds
)
{
	DrawRectangleRounded(
		bounds,
		0.06f,
		8,
		(Color){ 7, 19, 31, 255 }
	);

	DrawRectangleRoundedLinesEx(
		bounds,
		0.06f,
		8,
		1.5f,
		(Color){ 78, 150, 185, 210 }
	);

	DrawText(
		"ALERT FEED",
		(int)bounds.x + 14,
		(int)bounds.y + 12,
		16,
		(Color){ 105, 215, 255, 255 }
	);

	DrawLine(
		(int)bounds.x + 14,
		(int)bounds.y + 36,
		(int)(bounds.x + bounds.width - 14),
		(int)bounds.y + 36,
		(Color){ 45, 82, 105, 255 }
	);

	if (alertCount == 0)
	{
		DrawText(
			"No alerts yet",
			(int)bounds.x + 14,
			(int)bounds.y + 52,
			13,
			(Color){ 145, 175, 195, 255 }
		);

		return;
	}

	const int visibleAlertCount =
		alertCount < 3 ? alertCount : 3;

	for (int i = 0; i < visibleAlertCount; i++)
	{
		const int rowY =
			(int)bounds.y + 50 + (i * 24);

		const Color severityColor =
			GetAlertSeverityColor(alerts[i].severity);

		DrawCircle(
			(int)bounds.x + 20,
			rowY + 6,
			5.0f,
			severityColor
		);

		DrawText(
			GetAlertSeverityName(alerts[i].severity),
			(int)bounds.x + 30,
			rowY,
			10,
			severityColor
		);

		DrawText(
			alerts[i].message,
			(int)bounds.x + 78,
			rowY,
			10,
			RAYWHITE
		);
	}
}





	 
