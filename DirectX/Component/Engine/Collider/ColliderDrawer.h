﻿#pragma once

#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"

class LineRenderer3D;

class ColliderDrawer {
private:
    ColliderDrawer() = delete;
    ~ColliderDrawer() = delete;

public:
    static void drawOBB(
        LineRenderer3D& drawer,
        const OBB& obb,
        const Vector3& color = ColorPalette::lightGreen
    );
};
