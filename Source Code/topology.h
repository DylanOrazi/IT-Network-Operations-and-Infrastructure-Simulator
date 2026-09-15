#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "raylib.h"
#include "device.h"
#include "connection.h"

void DrawNetworkTopology(
    const Device devices[],
    int deviceCount,
    const Connection connections[],
    int connectionCount,
    Rectangle bounds,
    float animationTime
);

#endif