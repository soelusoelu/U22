#include "ColliderDrawer.h"
#include "../../../Engine/DebugManager/DebugUtility/LineRenderer/LineRenderer3D.h"
#include "../../../Math/Math.h"

void ColliderDrawer::drawOBB(LineRenderer3D& drawer, const OBB& obb) {
    const auto& center = obb.center;
    const auto& rot = obb.rotation;
    const auto& extents = obb.extents;

    Vector3 ftr = extents;
    ftr = Vector3::transform(ftr, rot) + center;
    Vector3 fbr = Vector3(extents.x, -extents.y, extents.z);
    fbr = Vector3::transform(fbr, rot) + center;
    Vector3 ftl = Vector3(-extents.x, extents.y, extents.z);
    ftl = Vector3::transform(ftl, rot) + center;
    Vector3 fbl = Vector3(-extents.x, -extents.y, extents.z);
    fbl = Vector3::transform(fbl, rot) + center;

    Vector3 btr = Vector3(extents.x, extents.y, -extents.z);
    btr = Vector3::transform(btr, rot) + center;
    Vector3 bbr = Vector3(extents.x, -extents.y, -extents.z);
    bbr = Vector3::transform(bbr, rot) + center;
    Vector3 btl = Vector3(-extents.x, extents.y, -extents.z);
    btl = Vector3::transform(btl, rot) + center;
    Vector3 bbl = -extents;
    bbl = Vector3::transform(bbl, rot) + center;

    drawer.renderLine(ftr, fbr, ColorPalette::lightGreen);
    drawer.renderLine(ftr, ftl, ColorPalette::lightGreen);
    drawer.renderLine(ftl, fbl, ColorPalette::lightGreen);
    drawer.renderLine(fbl, fbr, ColorPalette::lightGreen);

    drawer.renderLine(btr, bbr, ColorPalette::lightGreen);
    drawer.renderLine(btr, btl, ColorPalette::lightGreen);
    drawer.renderLine(btl, bbl, ColorPalette::lightGreen);
    drawer.renderLine(bbl, bbr, ColorPalette::lightGreen);

    drawer.renderLine(ftr, btr, ColorPalette::lightGreen);
    drawer.renderLine(ftl, btl, ColorPalette::lightGreen);
    drawer.renderLine(fbr, bbr, ColorPalette::lightGreen);
    drawer.renderLine(fbl, bbl, ColorPalette::lightGreen);
}
