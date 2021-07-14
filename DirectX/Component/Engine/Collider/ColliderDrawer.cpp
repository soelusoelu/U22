﻿#include "ColliderDrawer.h"
#include "../../../Engine/DebugManager/DebugUtility/LineRenderer/LineRenderer3D.h"
#include "../../../Math/Math.h"

void ColliderDrawer::drawOBB(
    LineRenderer3D& drawer,
    const OBB& obb,
    const Vector3& color
) {
    auto points = BoxHelper::getPoints(obb);

    const auto& ftr = points[BoxConstantGroup::BOX_BACK_TOP_RIGHT];
    const auto& fbr = points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT];
    const auto& ftl = points[BoxConstantGroup::BOX_BACK_TOP_LEFT];
    const auto& fbl = points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT];
    const auto& btr = points[BoxConstantGroup::BOX_FORE_TOP_RIGHT];
    const auto& bbr = points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT];
    const auto& btl = points[BoxConstantGroup::BOX_FORE_TOP_LEFT];
    const auto& bbl = points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT];

    drawer.renderLine(ftr, fbr, color);
    drawer.renderLine(ftr, ftl, color);
    drawer.renderLine(ftl, fbl, color);
    drawer.renderLine(fbl, fbr, color);

    drawer.renderLine(btr, bbr, color);
    drawer.renderLine(btr, btl, color);
    drawer.renderLine(btl, bbl, color);
    drawer.renderLine(bbl, bbr, color);

    drawer.renderLine(ftr, btr, color);
    drawer.renderLine(ftl, btl, color);
    drawer.renderLine(fbr, bbr, color);
    drawer.renderLine(fbl, bbl, color);
}
