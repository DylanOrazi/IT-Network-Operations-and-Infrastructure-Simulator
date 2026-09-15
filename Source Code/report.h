#ifndef REPORT_H
#define REPORT_H

#include "device.h"
#include "alert.h"
#include "incident.h"
#include "activity_log.h"

bool ExportSessionReport(
	const Device devices[],
	int deviceCount,
	const Alert alerts[],
	int alertCount,
	const Incident incidents[],
	int incidentCount,
	const ActivityLogEntry activityLog[],
	int activityLogCount,
	const char* filePath
);

#endif
