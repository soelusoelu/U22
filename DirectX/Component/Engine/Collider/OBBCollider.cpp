#include "OBBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"

OBBCollider::OBBCollider()
    : Collider()
    , mOBB()
{
}

OBBCollider::~OBBCollider() = default;

void OBBCollider::start() {
    Collider::start();

    auto mesh = getComponent<MeshComponent>();
    mOBB = OBBCreater::create(mesh->getMesh()->getMeshVerticesPosition(0));
}

void OBBCollider::lateUpdate() {
    Collider::lateUpdate();

    renderCollision();
}

void OBBCollider::renderCollision() const {
    const auto& center = mOBB.center;
    const auto& rot = mOBB.rotation;
    const auto& extents = mOBB.extents;

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

    Debug::renderLine(ftr, fbr, ColorPalette::lightGreen);
    Debug::renderLine(ftr, ftl, ColorPalette::lightGreen);
    Debug::renderLine(ftl, fbl, ColorPalette::lightGreen);
    Debug::renderLine(fbl, fbr, ColorPalette::lightGreen);

    Debug::renderLine(btr, bbr, ColorPalette::lightGreen);
    Debug::renderLine(btr, btl, ColorPalette::lightGreen);
    Debug::renderLine(btl, bbl, ColorPalette::lightGreen);
    Debug::renderLine(bbl, bbr, ColorPalette::lightGreen);

    Debug::renderLine(ftr, btr, ColorPalette::lightGreen);
    Debug::renderLine(ftl, btl, ColorPalette::lightGreen);
    Debug::renderLine(fbr, bbr, ColorPalette::lightGreen);
    Debug::renderLine(fbl, bbl, ColorPalette::lightGreen);

    //Debug::renderLine(center, center + Vector3::right, ColorPalette::lightGreen);
    //Debug::renderLine(center, center + Vector3::up, ColorPalette::lightGreen);
    //Debug::renderLine(center, center + Vector3::back, ColorPalette::lightGreen);
}
