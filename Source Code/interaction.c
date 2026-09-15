#include "interaction.h"

int GetClickedDeviceId(
	const Device devices[],
	int deviceCount
)
{
	if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		return -1;
	}

	const Vector2 mousePosition = GetMousePosition();

	const float nodeWidth = 160.0f;
	const float nodeHeight = 60.0f;

	for (int i = 0; i < deviceCount; i++)
	{
		const Rectangle nodeRectangle =
		{
			devices[i].topologyPosition.x - (nodeWidth / 2.0f),
			devices[i].topologyPosition.y - (nodeHeight / 2.0f),
			nodeWidth,
			nodeHeight
		};

		if (CheckCollisionPointRec(
			mousePosition,
			nodeRectangle
		))
		{
			return devices[i].id;
		}
	}

	return -1;
}

const Device* FindDeviceByIdForInteraction(
	const Device devices[],
	int deviceCount,
	int deviceId
)
{
	for (int i = 0; i < deviceCount; i++)
	{
		if (devices[i].id == deviceId)
		{
			return &devices[i];
		}
	}

	return NULL;
}

void DrawSelectedDeviceHighlight(
	const Device* device
)
{
	if (device == NULL)
	{
		return;
	}

	const float nodeWidth = 160.0f;
	const float nodeHeight = 60.0f;

	const Rectangle highlightRectangle =
	{
		device->topologyPosition.x - (nodeWidth / 2.0f) - 6.0f,
		device->topologyPosition.y - (nodeHeight / 2.0f) - 6.0f,
		nodeWidth + 12.0f,
		nodeHeight + 12.0f
	};

	const Color statusColor =
		GetDeviceStatusColor(device->status);

	DrawRectangleRoundedLinesEx(
		highlightRectangle,
		0.18f,
		8,
		3.0f,
		RAYWHITE
	);

	DrawRectangleRoundedLinesEx(
		(Rectangle)
		{
			highlightRectangle.x - 4.0f,
			highlightRectangle.y - 4.0f,
			highlightRectangle.width + 8.0f,
			highlightRectangle.height + 8.0f
		},
		0.18f,
		8,
		1.0f,
		ColorAlpha(statusColor, 0.65f)
	);
}

void DrawDeviceDetailsPanel(
	const Device* device
)
{
	if (device == NULL)
	{
		return;
	}

	const Rectangle panel =
	{
		965.0f,
		205.0f,
		215.0f,
		190.0f
	};

	const Color statusColor =
		GetDeviceStatusColor(device->status);

	DrawRectangleRounded(
		panel,
		0.08f,
		8,
		(Color) {
		9, 23, 37, 245
	}
	);

	DrawRectangleRoundedLinesEx(
		panel,
		0.08f,
		8,
		2.0f,
		statusColor
	);

	DrawText(
		"DEVICE DETAILS",
		982,
		220,
		16,
		(Color) {
		90, 210, 255, 255
	}
	);

	DrawLine(
		982,
		245,
		1160,
		245,
		(Color) {
		50, 88, 110, 255
	}
	);

	DrawText(
		device->name,
		982,
		258,
		17,
		RAYWHITE
	);

	DrawText(
		GetDeviceTypeName(device->type),
		982,
		284,
		14,
		(Color) {
		165, 190, 205, 255
	}
	);

	DrawText(
		device->ipAddress,
		982,
		306,
		14,
		(Color) {
		165, 190, 205, 255
	}
	);

	DrawText(
		GetDeviceStatusName(device->status),
		982,
		331,
		15,
		statusColor
	);

	DrawText(
		TextFormat(
			"Health: %.0f%%",
			device->healthPercent
		),
		982,
		356,
		14,
		RAYWHITE
	);

	DrawText(
		TextFormat(
			"Latency: %.1f ms",
			device->latencyMilliseconds
		),
		982,
		376,
		14,
		RAYWHITE
	);
}

