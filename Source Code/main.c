#include <stdio.h>

#include "raylib.h"
#include "device.h"
#include "connection.h"
#include "topology.h"
#include "interaction.h" 
#include "simulation.h"
#include "alert.h" 
#include "incident.h"
#include "technician.h"
#include "activity_log.h"
#include "report.h" 

#define STARTER_DEVICE_COUNT 6
#define STARTER_CONNECTION_COUNT 5

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(
        screenWidth,
        screenHeight,
        "IT Network Operations and Infrastructure Simulator"
    );

    SetTargetFPS(60);

    Device devices[STARTER_DEVICE_COUNT];

    devices[0] = CreateDevice(
        1,
        "EDGE-RTR-01",
        "192.168.1.1",
        DEVICE_ROUTER,
        STATUS_ONLINE,
        (Vector2) { 640.0f, 250.0f },
        -1
    );

    devices[1] = CreateDevice(
        2,
        "SEC-FW-01",
        "192.168.1.2",
        DEVICE_FIREWALL,
        STATUS_ONLINE,
        (Vector2) { 640.0f, 340.0f },
        1
    );

    devices[2] = CreateDevice(
        3,
        "CORE-SW-01",
        "192.168.1.3",
        DEVICE_CORE_SWITCH,
        STATUS_WARNING,
        (Vector2) { 640.0f, 430.0f },
        2
    );

    devices[3] = CreateDevice(
        4,
        "APP-SRV-01",
        "192.168.10.10",
        DEVICE_SERVER,
        STATUS_ONLINE,
        (Vector2) { 430.0f, 530.0f },
        3
    );

    devices[4] = CreateDevice(
        5,
        "BACKUP-SRV-01",
        "192.168.10.20",
        DEVICE_BACKUP_SERVER,
        STATUS_MAINTENANCE,
        (Vector2) { 640.0f, 530.0f },
        3
    );

    devices[5] = CreateDevice(
        6,
        "ACCESS-SW-01",
        "192.168.20.1",
        DEVICE_ACCESS_SWITCH,
        STATUS_ONLINE,
        (Vector2) { 850.0f, 530.0f },
        3
    );

    devices[0].latencyMilliseconds = 4.2f;

    devices[1].cpuUsagePercent = 24.0f;
    devices[1].memoryUsagePercent = 38.0f;

    devices[2].healthPercent = 78.0f;
    devices[2].latencyMilliseconds = 32.5f;
    devices[2].packetLossPercent = 3.4f;

    devices[3].cpuUsagePercent = 42.0f;
    devices[3].memoryUsagePercent = 61.0f;
    devices[3].diskUsagePercent = 57.0f;

    devices[4].isInMaintenance = true;
    devices[4].healthPercent = 91.0f;

    devices[5].cpuUsagePercent = 16.0f;
    devices[5].memoryUsagePercent = 29.0f;

    Connection connections[STARTER_CONNECTION_COUNT];

    connections[0] = CreateConnection(
        1,
        1,
        2,
        CONNECTION_HEALTHY,
        1000.0f
    );

    connections[1] = CreateConnection(
        2,
        2,
        3,
        CONNECTION_HEALTHY,
        1000.0f
    );

    connections[2] = CreateConnection(
        3,
        3,
        4,
        CONNECTION_HEALTHY,
        1000.0f
    );

    connections[3] = CreateConnection(
        4,
        3,
        5,
        CONNECTION_MAINTENANCE,
        1000.0f
    );

    connections[4] = CreateConnection(
        5,
        3,
        6,
        CONNECTION_DEGRADED,
        1000.0f
    );

    connections[0].currentTrafficMegabits = 285.0f;
    connections[0].latencyMilliseconds = 4.2f;

    connections[1].currentTrafficMegabits = 412.0f;
    connections[1].latencyMilliseconds = 5.8f;

    connections[2].currentTrafficMegabits = 328.0f;
    connections[2].latencyMilliseconds = 7.1f;

    connections[3].currentTrafficMegabits = 0.0f;
    connections[3].latencyMilliseconds = 0.0f;

    connections[4].currentTrafficMegabits = 640.0f;
    connections[4].latencyMilliseconds = 32.5f;
    connections[4].packetLossPercent = 3.4f;

    int healthyConnectionCount = 0;
    int degradedConnectionCount = 0;
    int unavailableConnectionCount = 0;

    for (int i = 0; i < STARTER_CONNECTION_COUNT; i++)
    {
        switch (connections[i].status)
        {
            case CONNECTION_HEALTHY:
                healthyConnectionCount++;
                break;

            case CONNECTION_DEGRADED:
                degradedConnectionCount++;
                break;

            case CONNECTION_DOWN:
            case CONNECTION_MAINTENANCE:
                unavailableConnectionCount++;
                break;

            default:
                break;
        }
    }

    bool showTopologyView = false; 
    int selectedDeviceId = -1;

    char actionFeedbackMessage[160] = "Click a device to inspect";
    float lastActionTime = -10.0f;

    const char* sessionReportPath = "reports\\network_session_report.txt";

    ActivityLogEntry activityLog[MAX_ACTIVITY_LOG_ENTRIES];
    int activityLogCount = 0;

    InitializeActivityLog(
        activityLog,
        &activityLogCount
    );

    Alert alerts[MAX_ALERTS];
    int alertCount = 0;

    InitializeAlertFeed(
        alerts,
        &alertCount
    );

    Incident incidents[MAX_INCIDENTS];
    int incidentCount = 0;

    InitializeIncidents(
        incidents,
        &incidentCount
    );

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_T))
        {
            showTopologyView = !showTopologyView;
        }

        UpdateDeviceMetrics(
            devices,
            STARTER_DEVICE_COUNT,
            GetFrameTime()
        );

        UpdateAlertFeedFromDevices(
            alerts,
            &alertCount,
            devices,
            STARTER_DEVICE_COUNT,
            (float)GetTime()
        );

        UpdateIncidentsFromDevices(
            incidents,
            &incidentCount,
            devices,
            STARTER_DEVICE_COUNT,
            (float)GetTime()
        );

        if (selectedDeviceId != -1 &&
            GetTime() - lastActionTime > 3.0f)
        {
            snprintf(
                actionFeedbackMessage,
                160,
                "Device selected - choose an action"
            );
        }

        if (showTopologyView &&
            IsKeyPressed(KEY_X))
        {
            const bool exportSucceeded =
                ExportSessionReport(
                    devices,
                    STARTER_DEVICE_COUNT,
                    alerts,
                    alertCount,
                    incidents,
                    incidentCount,
                    activityLog,
                    activityLogCount,
                    sessionReportPath
                );

            if (exportSucceeded)
            {
                snprintf(
                    actionFeedbackMessage,
                    160,
                    "Session report exported to reports folder"
                );

                RecordAlert(
                    alerts,
                    &alertCount,
                    ALERT_INFO,
                    "Session report exported",
                    (float)GetTime()
                );
            }
            else
            {
                snprintf(
                    actionFeedbackMessage,
                    160,
                    "Report export failed"
                );

                RecordAlert(
                    alerts,
                    &alertCount,
                    ALERT_WARNING,
                    "Session report export failed",
                    (float)GetTime()
                );
            }
        }

        BeginDrawing();

        ClearBackground((Color) { 10, 18, 28, 255 });

        DrawText(
            "IT NETWORK OPERATIONS AND INFRASTRUCTURE SIMULATOR",
            80,
            80,
            28,
            (Color) {
            90, 210, 255, 255
        }
        );

        DrawRectangleLines(
            80,
            190,
            1120,
            420,
            (Color) { 60, 110, 140, 255 }
        );

        if (!showTopologyView)
        {

            DrawText(
                "DEVICE",
                120,
                225,
                16,
                (Color) {
                120, 155, 178, 255
            }
            );

            DrawText(
                "TYPE",
                410,
                225,
                16,
                (Color) {
                120, 155, 178, 255
            }
            );

            DrawText(
                "IP ADDRESS",
                650,
                225,
                16,
                (Color) {
                120, 155, 178, 255
            }
            );

            DrawText(
                "STATUS",
                860,
                225,
                16,
                (Color) {
                120, 155, 178, 255
            }
            );

            DrawText(
                "HEALTH",
                1040,
                225,
                16,
                (Color) {
                120, 155, 178, 255
            }
            );

            DrawLine(
                110,
                252,
                1170,
                252,
                (Color) {
                45, 82, 105, 255
            }
            );

            for (int i = 0; i < STARTER_DEVICE_COUNT; i++)
            {
                const int rowY = 280 + (i * 48);
                const Color statusColor =
                    GetDeviceStatusColor(devices[i].status);

                DrawCircle(
                    120,
                    rowY + 8,
                    7.0f,
                    statusColor
                );

                DrawText(
                    devices[i].name,
                    145,
                    rowY,
                    18,
                    RAYWHITE
                );

                DrawText(
                    GetDeviceTypeName(devices[i].type),
                    410,
                    rowY,
                    18,
                    (Color) {
                    180, 204, 219, 255
                }
                );

                DrawText(
                    devices[i].ipAddress,
                    650,
                    rowY,
                    18,
                    (Color) {
                    180, 204, 219, 255
                }
                );

                DrawText(
                    GetDeviceStatusName(devices[i].status),
                    860,
                    rowY,
                    18,
                    statusColor
                );

                DrawText(
                    TextFormat("%.0f%%", devices[i].healthPercent),
                    1040,
                    rowY,
                    18,
                    statusColor
                );

                DrawText(
                    TextFormat(
                        "CPU %.0f%%  MEM %.0f%%  LAT %.1fms",
                        devices[i].cpuUsagePercent,
                        devices[i].memoryUsagePercent,
                        devices[i].latencyMilliseconds
                    ),
                    145,
                    rowY + 22,
                    12,
                    (Color) { 112, 145, 165, 255 }
                );
            }

            DrawLine(
                110,
                566,
                1170,
                566,
                (Color) {
                45, 82, 105, 255
            }
            );

            DrawText(
                TextFormat(
                    "NETWORK LINKS: %d",
                    STARTER_CONNECTION_COUNT
                ),
                120,
                580,
                16,
                (Color) {
                150, 184, 205, 255
            }
            );

            DrawCircle(
                380,
                589,
                6.0f,
                GetConnectionStatusColor(CONNECTION_HEALTHY)
            );

            DrawText(
                TextFormat(
                    "HEALTHY: %d",
                    healthyConnectionCount
                ),
                396,
                580,
                16,
                GetConnectionStatusColor(CONNECTION_HEALTHY)
            );

            DrawCircle(
                610,
                589,
                6.0f,
                GetConnectionStatusColor(CONNECTION_DEGRADED)
            );

            DrawText(
                TextFormat(
                    "DEGRADED: %d",
                    degradedConnectionCount
                ),
                626,
                580,
                16,
                GetConnectionStatusColor(CONNECTION_DEGRADED)
            );

            DrawCircle(
                870,
                589,
                6.0f,
                GetConnectionStatusColor(CONNECTION_MAINTENANCE)
            );

            DrawText(
                TextFormat(
                    "UNAVAILABLE: %d",
                    unavailableConnectionCount
                ),
                886,
                580,
                16,
                GetConnectionStatusColor(CONNECTION_MAINTENANCE)
            );

        }
        else
        {
            int clickedDeviceId =
                GetClickedDeviceId(
                    devices,
                    STARTER_DEVICE_COUNT
                );

            if (clickedDeviceId != -1)
            {
                selectedDeviceId = clickedDeviceId;
            }

            DrawNetworkTopology(
                devices,
                STARTER_DEVICE_COUNT,
                connections,
                STARTER_CONNECTION_COUNT,
                (Rectangle){ 81.0f, 191.0f, 1118.0f, 418.0f },
                (float)GetTime()
            );

            DrawAlertFeed(
                alerts,
                alertCount,
                (Rectangle){ 115.0f, 315.0f, 330.0f, 120.0f }
            );

            DrawIncidentPanel(
                incidents,
                incidentCount,
                (Rectangle){ 965.0f, 405.0f, 215.0f, 145.0f }
            );

            HandleTechnicianActions(
                devices,
                STARTER_DEVICE_COUNT,
                selectedDeviceId,
                alerts,
                &alertCount,
                (float)GetTime(),
                actionFeedbackMessage,
                160,
                &lastActionTime,
                activityLog,
                &activityLogCount

            );

            HandleTechnicianIncidentActions(
                incidents,
                incidentCount,
                selectedDeviceId,
                (float)GetTime(),
                lastActionTime
            );

            const Device* selectedDevice =
                FindDeviceByIdForInteraction(
                    devices,
                    STARTER_DEVICE_COUNT,
                    selectedDeviceId
                );

            DrawSelectedDeviceHighlight(
                selectedDevice
            );

            DrawDeviceDetailsPanel(
                selectedDevice
            );
        }

        DrawText(
            "Press ESC to close",
            80,
            650,
            18,
            GRAY
        );

        DrawText(
            showTopologyView
            ? "Press T to view device table"
            : "Press T to view network topology",
            930,
            650,
            16,
            (Color){ 90, 210, 255, 255 }
        );

        if (showTopologyView)
        {
            DrawText(
                selectedDeviceId != -1
                ? actionFeedbackMessage
                : "Click a device to inspect",
                455,
                650,
                15,
                (Color){ 145, 175, 195, 255 }
            );

            if (selectedDeviceId != -1)
            {
                DrawText(
                    "P Ping | D Ack | R Resolve | E Escalate | X Export",
                    455,
                    672,
                    13,
                    (Color){ 90, 210, 255, 255 }
                );

                DrawText(
                    TextFormat(
                        "Actions logged: %d",
                        GetActivityLogCount(activityLogCount)
                    ),
                    80,
                    672,
                    12,
                    (Color){ 110, 140, 160, 255 }
                );
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}