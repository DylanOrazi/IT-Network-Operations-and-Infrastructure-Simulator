#ifndef INCIDENT_H
#define INCIDENT_H

#include "raylib.h"
#include "device.h"

#define MAX_INCIDENTS 32
#define MAX_INCIDENT_TITLE_LENGTH 128

typedef enum IncidentSeverity
{
    INCIDENT_SEVERITY_INFO = 0,
    INCIDENT_SEVERITY_WARNING,
    INCIDENT_SEVERITY_CRITICAL
} IncidentSeverity;

typedef enum IncidentStatus
{
    INCIDENT_STATUS_OPEN = 0,
    INCIDENT_STATUS_ACKNOWLEDGED,
    INCIDENT_STATUS_ESCALATED,
    INCIDENT_STATUS_RESOLVED
} IncidentStatus;

typedef struct Incident
{
    int id;
    int deviceId;

    IncidentSeverity severity;
    IncidentStatus status;

    char title[MAX_INCIDENT_TITLE_LENGTH];

    float timeOpened;
    float timeUpdated;
    float timeResolved;

    bool isActive;
} Incident;

void InitializeIncidents(
    Incident incidents[],
    int* incidentCount
);

void UpdateIncidentsFromDevices(
    Incident incidents[],
    int* incidentCount,
    const Device devices[],
    int deviceCount,
    float currentTime
);

void DrawIncidentPanel(
    const Incident incidents[],
    int incidentCount,
    Rectangle bounds
);

bool AcknowledgeIncidentForDevice(
    Incident incidents[],
    int incidentCount,
    int deviceId,
    float currentTime
);

bool ResolveIncidentForDevice(
    Incident incidents[],
    int incidentCount,
    int deviceId,
    float currentTime
);

bool EscalateIncidentForDevice(
    Incident incidents[],
    int incidentCount,
    int deviceId,
    float currentTime
);

#endif