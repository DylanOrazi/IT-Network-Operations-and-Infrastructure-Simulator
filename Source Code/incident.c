#include "incident.h"

#include <stdio.h>
#include <string.h>

static int nextIncidentId = 1;

static const char* GetIncidentSeverityName(IncidentSeverity severity)
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

static const char* GetIncidentStatusName(IncidentStatus status)
{
    switch (status)
    {
        case INCIDENT_STATUS_OPEN:
            return "OPEN";

        case INCIDENT_STATUS_ACKNOWLEDGED:
            return "ACK";

        case INCIDENT_STATUS_ESCALATED:
            return "ESCALATED";

        case INCIDENT_STATUS_RESOLVED:
            return "RESOLVED";

        default:
            return "UNKNOWN";
    }
}

static Color GetIncidentSeverityColor(IncidentSeverity severity)
{
    switch (severity)
    {
    case INCIDENT_SEVERITY_INFO:
        return (Color) { 105, 210, 255, 255 };

    case INCIDENT_SEVERITY_WARNING:
        return (Color) { 255, 193, 77, 255 };

    case INCIDENT_SEVERITY_CRITICAL:
        return (Color) { 255, 82, 105, 255 };

    default:
        return GRAY;
    }
}

static int FindActiveIncidentIndexForDevice(
    const Incident incidents[],
    int incidentCount,
    int deviceId
)
{
    for (int i = 0; i < incidentCount; i++)
    {
        if (incidents[i].deviceId == deviceId &&
            incidents[i].status != INCIDENT_STATUS_RESOLVED &&
            incidents[i].isActive)
        {
            return i;
        }
    }

    return -1;
}

static void ShiftIncidentsDown(
    Incident incidents[]
)
{
    for (int i = MAX_INCIDENTS - 1; i > 0; i--)
    {
        incidents[i] = incidents[i - 1];
    }
}

static void OpenIncident(
    Incident incidents[],
    int* incidentCount,
    int deviceId,
    IncidentSeverity severity,
    const char* title,
    float currentTime
)
{
    ShiftIncidentsDown(incidents);

    incidents[0].id = nextIncidentId;
    incidents[0].deviceId = deviceId;
    incidents[0].severity = severity;
    incidents[0].status = INCIDENT_STATUS_OPEN;

    incidents[0].timeOpened = currentTime;
    incidents[0].timeUpdated = currentTime;
    incidents[0].timeResolved = 0.0f;

    incidents[0].isActive = true;

    snprintf(
        incidents[0].title,
        MAX_INCIDENT_TITLE_LENGTH,
        "%s",
        title
    );

    nextIncidentId++;

    if (*incidentCount < MAX_INCIDENTS)
    {
        (*incidentCount)++;
    }
}

static void UpdateExistingIncident(
    Incident* incident,
    IncidentSeverity severity,
    const char* title,
    float currentTime
)
{
    incident->severity = severity;
    incident->timeUpdated = currentTime;

    snprintf(
        incident->title,
        MAX_INCIDENT_TITLE_LENGTH,
        "%s",
        title
    );
}

static void ResolveIncident(
    Incident* incident,
    float currentTime
)
{
    incident->status = INCIDENT_STATUS_RESOLVED;
    incident->timeUpdated = currentTime;
    incident->timeResolved = currentTime;
    incident->isActive = false;
}

bool AcknowledgeIncidentForDevice(
    Incident incidents[],
    int incidentCount,
    int deviceId,
    float currentTime
)
{
    const int incidentIndex =
        FindActiveIncidentIndexForDevice(
            incidents,
            incidentCount,
            deviceId
        );

    if (incidentIndex < 0)
    {
        return false;
    }

    if (incidents[incidentIndex].status == INCIDENT_STATUS_OPEN)
    {
        incidents[incidentIndex].status =
            INCIDENT_STATUS_ACKNOWLEDGED;
    }

    incidents[incidentIndex].timeUpdated = currentTime;

    return true;
}

bool ResolveIncidentForDevice(
    Incident incidents[],
    int incidentCount,
    int deviceId,
    float currentTime
)
{
    const int incidentIndex =
        FindActiveIncidentIndexForDevice(
            incidents,
            incidentCount,
            deviceId
        );

    if (incidentIndex < 0)
    {
        return false;
    }

    ResolveIncident(
        &incidents[incidentIndex],
        currentTime
    );

    return true;
}

bool EscalateIncidentForDevice(
    Incident incidents[],
    int incidentCount,
    int deviceId,
    float currentTime
)
{
    const int incidentIndex =
        FindActiveIncidentIndexForDevice(
            incidents,
            incidentCount,
            deviceId
        );

    if (incidentIndex < 0)
    {
        return false;
    }

    incidents[incidentIndex].status =
        INCIDENT_STATUS_ESCALATED;

    incidents[incidentIndex].severity =
        INCIDENT_SEVERITY_CRITICAL;

    incidents[incidentIndex].timeUpdated = currentTime;

    return true;
}

void InitializeIncidents( 
    Incident incidents[],
    int* incidentCount
)
{
    memset(
        incidents,
        0,
        sizeof(Incident) * MAX_INCIDENTS
    );

    *incidentCount = 0;
    nextIncidentId = 1;
}

void UpdateIncidentsFromDevices(
    Incident incidents[],
    int* incidentCount,
    const Device devices[],
    int deviceCount,
    float currentTime
)
{
    for (int i = 0; i < deviceCount; i++)
    {
        const Device* device = &devices[i];

        if (device->status == STATUS_MAINTENANCE ||
            device->isInMaintenance)
        {
            continue;
        }

        const int activeIncidentIndex =
            FindActiveIncidentIndexForDevice(
                incidents,
                *incidentCount,
                device->id
            );

        bool shouldOpenIncident = false;
        IncidentSeverity severity = INCIDENT_SEVERITY_INFO;

        char title[MAX_INCIDENT_TITLE_LENGTH];

        if (device->status == STATUS_CRITICAL)
        {
            shouldOpenIncident = true;
            severity = INCIDENT_SEVERITY_CRITICAL;

            snprintf(
                title,
                MAX_INCIDENT_TITLE_LENGTH,
                "%s critical health condition",
                device->name
            );
        }
        else if (device->packetLossPercent > 5.0f)
        {
            shouldOpenIncident = true;
            severity = INCIDENT_SEVERITY_CRITICAL;

            snprintf(
                title,
                MAX_INCIDENT_TITLE_LENGTH,
                "%s packet loss above threshold",
                device->name
            );
        }
        else if (device->status == STATUS_WARNING)
        {
            shouldOpenIncident = true;
            severity = INCIDENT_SEVERITY_WARNING;

            snprintf(
                title,
                MAX_INCIDENT_TITLE_LENGTH,
                "%s warning health condition",
                device->name
            );
        }
        else if (device->latencyMilliseconds > 60.0f)
        {
            shouldOpenIncident = true;
            severity = INCIDENT_SEVERITY_WARNING;

            snprintf(
                title,
                MAX_INCIDENT_TITLE_LENGTH,
                "%s latency above threshold",
                device->name
            );
        }
        else if (device->healthPercent < 70.0f)
        {
            shouldOpenIncident = true;
            severity = INCIDENT_SEVERITY_WARNING;

            snprintf(
                title,
                MAX_INCIDENT_TITLE_LENGTH,
                "%s health below 70%%",
                device->name
            );
        }

        if (shouldOpenIncident)
        {
            if (activeIncidentIndex >= 0)
            {
                UpdateExistingIncident(
                    &incidents[activeIncidentIndex],
                    severity,
                    title,
                    currentTime
                );
            }
            else
            {
                OpenIncident(
                    incidents,
                    incidentCount,
                    device->id,
                    severity,
                    title,
                    currentTime
                );
            }
        }
        else
        {
            if (activeIncidentIndex >= 0)
            {
                ResolveIncident(
                    &incidents[activeIncidentIndex],
                    currentTime
                );
            }
        }
    }
}

void DrawIncidentPanel(
    const Incident incidents[],
    int incidentCount,
    Rectangle bounds
)
{
    DrawRectangleRounded(
        bounds,
        0.06f,
        8,
        (Color) {
        7, 19, 31, 225
    }
    );

    DrawRectangleRoundedLinesEx(
        bounds,
        0.06f,
        8,
        1.5f,
        (Color) {
        78, 150, 185, 210
    }
    );

    DrawText(
        "INCIDENTS",
        (int)bounds.x + 14,
        (int)bounds.y + 12,
        16,
        (Color) {
        105, 215, 255, 255
    }
    );

    DrawLine(
        (int)bounds.x + 14,
        (int)bounds.y + 36,
        (int)(bounds.x + bounds.width - 14),
        (int)bounds.y + 36,
        (Color) {
        45, 82, 105, 255
    }
    );

    int openIncidentCount = 0;

    for (int i = 0; i < incidentCount; i++)
    {
        if (incidents[i].status != INCIDENT_STATUS_OPEN &&
            incidents[i].isActive) 
        {
            openIncidentCount++;
        }
    }

    DrawText(
        TextFormat("Open: %d", openIncidentCount),
        (int)bounds.x + 14,
        (int)bounds.y + 48,
        12,
        RAYWHITE
    );

    if (incidentCount == 0)
    {
        DrawText(
            "No incidents yet",
            (int)bounds.x + 14,
            (int)bounds.y + 72,
            12,
            (Color) {
            145, 175, 195, 255
        }
        );

        return;
    }

    int drawnCount = 0;

    for (int i = 0; i < incidentCount && drawnCount < 3; i++)
    {
        const int rowY =
            (int)bounds.y + 72 + (drawnCount * 24);

        const Color severityColor =
            GetIncidentSeverityColor(incidents[i].severity);

        DrawCircle(
            (int)bounds.x + 20,
            rowY + 6,
            5.0f,
            severityColor
        );

        DrawText(
            GetIncidentSeverityName(incidents[i].severity),
            (int)bounds.x + 32,
            rowY,
            10,
            severityColor
        );

        DrawText(
            GetIncidentStatusName(incidents[i].status),
            (int)bounds.x + 112,
            rowY,
            10,
            incidents[i].status == INCIDENT_STATUS_OPEN
            ? RAYWHITE
            : (Color) { 120, 150, 170, 255 }
        );

        DrawText(
            TextFormat("#%d", incidents[i].id),
            (int)bounds.x + 188,
            rowY,
            10,
            (Color) {
            145, 175, 195, 255
        }
        );

        drawnCount++;
    }
}