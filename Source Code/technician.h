#ifndef TECHNICIAN_H
#define TECHNICIAN_H

#include "device.h"
#include "alert.h"
#include "incident.h" 
#include "activity_log.h" 

void HandleTechnicianActions(
	Device devices[],
	int deviceCount,
	int selectedDeviceId,
	Alert alerts[],
	int* alertCount,
	float currentTime,
	char actionFeedbackMessage[],
	int actionFeedBackMessageLength,
	float* lastActionTime,
	ActivityLogEntry activityLog[],
	int* activityLogCount

);

void HandleTechnicianIncidentActions(
	Incident incidents[],
	int incidentCount,
	int selectedDeviceId,
	float currentTime,
	float lastActionTime
);

#endif
