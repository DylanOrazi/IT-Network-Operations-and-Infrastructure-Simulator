#ifndef ALERT_H
#define ALERT_H

#include "raylib.h"
#include "device.h"

#define MAX_ALERTS 32
#define MAX_ALERT_MESSAGE_LENGTH 128

typedef enum AlertSeverity
{
	ALERT_INFO = 0,
	ALERT_WARNING,
	ALERT_CRITICAL
} AlertSeverity;

typedef struct Alert
{
	int id;
	AlertSeverity severity;
	char message[MAX_ALERT_MESSAGE_LENGTH];
	float timeCreated;
	bool isActive;
} Alert;

void InitializeAlertFeed(
	Alert alerts[],
	int* alertCount
);

void UpdateAlertFeedFromDevices(
	Alert alerts[],
	int* alertCount,
	const Device devices[],
	int deviceCount,
	float currentTime
);

void DrawAlertFeed(
	const Alert alerts[],
	int alertCount,
	Rectangle bounds
);

void RecordAlert(
	Alert alerts[],
	int* alertCount,
	AlertSeverity severity,
	const char* message,
	float currentTime
);

#endif 



