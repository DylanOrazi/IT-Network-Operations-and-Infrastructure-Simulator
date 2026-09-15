#include "topology.h"

#include <math.h>

static const Device* FindDeviceById(
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

static void DrawTopologyConnection(
	const Device* source,
	const Device* destination,
	const Connection* connection,
	float animationTime
)
{
	const Color connectionColor =
		GetConnectionStatusColor(connection->status);

	const Vector2 startPosition = {
		source->topologyPosition.x,
		source->topologyPosition.y + 30.0f
	};

	const Vector2 endPosition = {
		destination->topologyPosition.x,
		destination->topologyPosition.y - 30.0f
	};

	DrawLineEx(
		startPosition,
		endPosition,
		4.0f,
		ColorAlpha(connectionColor, 0.35f)
	);

	DrawLineEx(
		startPosition,
		endPosition,
		1.5f,
		connectionColor
	);

	if (connection->isActive)
	{
		const float packetProgress =
			fmodf(
				animationTime * 0.35f +
				((float)connection->id * 0.17f),
				1.0f
			);

		const Vector2 packetPosition = {
			startPosition.x +
			((endPosition.x - startPosition.x) * packetProgress),

			startPosition.y +
			((endPosition.y - startPosition.y) * packetProgress)
		};

		DrawCircle(
			(int)packetPosition.x,
			(int)packetPosition.y,
			5.0f,
			RAYWHITE
		);

		DrawCircleLines(
			(int)packetPosition.x,
			(int)packetPosition.y,
			8.0f,
			connectionColor
		);
	}
}

static void  DrawTopologyDeviceNode(const Device* device)
{
	const float nodeWidth = 160.0f;
	const float nodeHeight = 60.0f;

	const Rectangle nodeRectangle = {
		device->topologyPosition.x - (nodeWidth / 2.0f),
		device->topologyPosition.y - (nodeHeight / 2.0f),
		nodeWidth,
		nodeHeight
	};

	const Color statusColor =
		GetDeviceStatusColor(device->status);

	DrawRectangleRounded(
		nodeRectangle,
		0.18f,
		8,
		(Color) {
		18, 34, 50, 245
	}
	);

	DrawRectangleRoundedLinesEx(
		nodeRectangle,
		0.18f,
		8,
		2.0f,
		statusColor
	);

	DrawCircle(
		(int)(nodeRectangle.x + 17.0f),
		(int)(nodeRectangle.y + 17.0f),
		6.0f,
		statusColor
	);

	DrawText(
		device->name,
		(int)(nodeRectangle.x + 30.0f),
		(int)(nodeRectangle.y + 8.0f),
		16,
		RAYWHITE
	);

	DrawText(
		GetDeviceTypeName(device->type),
		(int)(nodeRectangle.x + 12.0f),
		(int)(nodeRectangle.y + 34.0f),
		13,
		(Color) {
		145, 178, 198, 255
	}
	);

	DrawText(
		TextFormat("%.0f%", device->healthPercent),
		(int)(nodeRectangle.x + 118.0f),
		(int)(nodeRectangle.y + 34.0f),
		13,
		statusColor
	);
}

void DrawNetworkTopology(
	const Device devices[],
	int deviceCount,
	const Connection connections[],
	int connectionCount,
	Rectangle bounds,
	float animationTime
)
{ 
	DrawRectangleRec(
		bounds,
		(Color) { 8, 20, 33, 170 }
	);

	for (
		float x = bounds.x + 20.0f;
		x < bounds.x + bounds.width;
		x += 40.0f
	)
	{
		DrawLine(
			(int)x,
			(int)bounds.y,
			(int)x,
			(int)(bounds.y + bounds.height),
			(Color) { 35, 70, 92, 60 }
		);
	}

	for (
		float y = bounds.y + 20.0f;
		y < bounds.y + bounds.height;
		y += 40.0f
		)
	{
		DrawLine(
			(int)bounds.x,
			(int)y,
			(int)(bounds.x + bounds.width),
			(int)y,
			(Color) { 35, 70, 92, 60 }
		);
	}

	DrawText(
		"LIVE INFRASTRUCTURE TOPOLOGY",
		(int)bounds.x + 18,
		(int)bounds.y + 14,
		16,
		(Color) { 108, 214, 255, 255 }
	);

	DrawText(
		"Animated packets represent active network traffic",
		(int)bounds.x + 18,
		(int)bounds.y + 38,
		13,
		(Color) { 125, 155, 175, 255 }
	);

	for (int i = 0; i < connectionCount; i++)
	{
		const Device* source =
			FindDeviceById(
				devices,
				deviceCount,
				connections[i].sourceDeviceId
			);

		const Device* destination =
			FindDeviceById(
				devices,
				deviceCount,
				connections[i].destinationDeviceId
			);

		if (source != NULL && destination != NULL)
		{
			DrawTopologyConnection(
				source,
				destination,
				&connections[i],
				animationTime
			);
		}
	}

	for (int i = 0; i < deviceCount; i++)
	{
		DrawTopologyDeviceNode(&devices[i]);
	}
}
		
