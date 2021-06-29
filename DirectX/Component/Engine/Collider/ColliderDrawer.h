#pragma once

#include "../../../Collision/Collision.h"

class LineRenderer3D;

class ColliderDrawer {
private:
    ColliderDrawer() = delete;
    ~ColliderDrawer() = delete;

public:
    static void drawOBB(LineRenderer3D& drawer, const OBB& obb);
};
