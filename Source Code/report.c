#include "report.h"

#include <stdio.h>
#include <direct.h>

static const char* GetAlertSeverityNameForReport(
	AlertSeverity severity
)
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

static const char* GetIncidentSeverityNameForReport(
	IncidentSeverity severity
)
{
	switch (severity)
	{
		case INCIDENT_SEVERITY_INFO:
			return "INFO";

		case INCIDENT_SEVERITY_WARNING:
			return "WARNING";

		case INCIDENT_SEVERITY_CRITICAL:
			return "CRITICAL";

		default:
			return "UNKNOWN";
	}
}

static const char* GetIncidentStatusNameForReport(
	IncidentStatus status
)
{
	switch (status)
	{
		case INCIDENT_STATUS_OPEN:
			return "OPEN";

		case INCIDENT_STATUS_ACKNOWLEDGED:
			return "ACKNOWLEDGED";

		case INCIDENT_STATUS_ESCALATED:
			return "ESCALATED";

		case INCIDENT_STATUS_RESOLVED:
			return "RESOLVED";

		default:
			return "UNKNOWN";
	}
}

static int CountOpenIncidents(
	const Incident incidents[],
	int incidentCount
)
{
	int count = 0;

	for (int i = 0; i < incidentCount; i++)
	{
		if (incidents[i].status != INCIDENT_STATUS_RESOLVED &&
			incidents[i].isActive)
		{
			count++;
		}
	}

	return count;
}

static int CountResolvedIncidents(
	const Incident incidents[],
	int incidentCount
)
{
	int count = 0;

	for (int i = 0; i < incidentCount; i++)
	{
		if (incidents[i].status == INCIDENT_STATUS_RESOLVED)
		{
			count++;
		}
	}

	return count;
}

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
)
{ 
	_mkdir("reports");

	FILE* reportFile = NULL;

	if (fopen_s(&reportFile, filePath, "w") != 0 ||
		reportFile == NULL)
	{
		return false;
	}

	fprintf(
		reportFile,
		"IT Network Operations and Infrastructure Simulator\n"
	);

	fprintf(
		reportFile,
		"Session Report\n"
	);

	fprintf(
		reportFile,
		"============================================================\n\n"
	);

	fprintf(
		reportFile,
		"SESSION SUMMARY\n"
	);

	fprintf(
		reportFile,
		"------------------------------------------------------------\n"
	);

	fprintf(
		reportFile,
		"Total devices monitored: %d\n",
		deviceCount
	);

	fprintf(
		reportFile,
		"Total alerts generated: %d\n",
		alertCount
	);

	fprintf(
		reportFile,
		"Total incidents tracked: %d\n",
		incidentCount
	);

	fprintf(
		reportFile,
		"Open incidents: %d\n",
		CountOpenIncidents(
			incidents,
			incidentCount
		)
	);

	fprintf(
		reportFile,
		"Resolved incidents: %d\n",
		CountResolvedIncidents(
			incidents,
			incidentCount
		)
	); 

	fprintf(
		reportFile,
		"Technician actions logged: %d\n\n",
		activityLogCount
	);

	fprintf(
		reportFile,
		"DEVICE HEALTH SUMMARY\n"
	);

	fprintf(
		reportFile,
		"------------------------------------------------------------\n"
	);
	
	for (int i = 0; i < deviceCount; i++)
	{
		fprintf(
			reportFile,
			"%s | %s | %s | Health %.0f%% | CPU %.0f%% | Memory %.0f%% | Latency %.1f ms | Packet Loss %.1f%%\n",
			devices[i].name,
			GetDeviceTypeName(devices[i].type),
			GetDeviceStatusName(devices[i].status),
			devices[i].healthPercent,
			devices[i].cpuUsagePercent,
			devices[i].memoryUsagePercent,
			devices[i].latencyMilliseconds,
			devices[i].packetLossPercent
		);
	}

	fprintf(
		reportFile,
		"\nALERT SUMMARY\n"
	);

	fprintf(
		reportFile,
		"------------------------------------------------------------\n"
	);

	if (alertCount == 0)
	{
		fprintf(
			reportFile,
			"No alerts recorded.\n"
		);
	}
	else
	{
		for (int i = 0; i < alertCount; i++)
		{
			fprintf(
				reportFile,
				"#%d | %.1fs | %s | %s\n",
				alerts[i].id,
				alerts[i].timeCreated,
				GetAlertSeverityNameForReport(alerts[i].severity),
				alerts[i].message
			);
		}
	}

	fprintf(
		reportFile,
		"\nINCIDENT SUMMARY\n"
	);

	fprintf(
		reportFile,
		"------------------------------------------------------------\n"
	);

	if (incidentCount == 0)
	{
		fprintf(
			reportFile,
			"No incidents recorded.\n"
		);
	}
	else
	{
		for (int i = 0; i < incidentCount; i++)
		{
			fprintf(
				reportFile,
				"#%d | Device ID %d | %s | %s | %s | Opened %.1fs | Updated %.1fs\n",
				incidents[i].id,
				incidents[i].deviceId,
				GetIncidentSeverityNameForReport(incidents[i].severity),
				GetIncidentStatusNameForReport(incidents[i].status),
				incidents[i].title,
				incidents[i].timeOpened,
				incidents[i].timeUpdated
			);
		}
	}

	fprintf(
		reportFile,
		"\nTECHNICIAN ACTIVITY LOG\n"
	);

	fprintf(
		reportFile,
		"------------------------------------------------------------\n"
	);

	if (activityLogCount == 0)
	{
		fprintf(
			reportFile,
			"No technician actions recorded.\n"
		);
	}
	else
	{
		for (int i = 0; i < activityLogCount; i++)
		{
			fprintf(
				reportFile,
				"#%d | %.1fs | %s | %s | %s | Status %s | Health %.0f%% | Latency %.1f ms | Packet Loss %.1f%%\n",
				activityLog[i].id,
				activityLog[i].timestampSeconds,
				activityLog[i].deviceName,
				activityLog[i].action,
				activityLog[i].result,
				GetDeviceStatusName(activityLog[i].deviceStatusAtAction),
				activityLog[i].healthPercentAtAction,
				activityLog[i].latencyMillisecondsAtAction,
				activityLog[i].packetLossPercentAtAction
			);
		}
	}

	fprintf(
		reportFile,
		"\nEND OF REPORT\n"
	);

	fclose(reportFile);

	return true;
}


		