#include "BulletShooter.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../../Collision/Collision.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../System/Window.h"
#include "../../../Transform/Transform3D.h"

BulletShooter::BulletShooter()
    : Component()
    , mCamera(nullptr)
    , mGun(nullptr)
{
}

BulletShooter::~BulletShooter() = default;

void BulletShooter::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();
}

void BulletShooter::update() {
    const auto& mouse = Input::mouse();
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    const auto center = Vector2(Window::width(), Window::height()) / 2.f;
    auto ray = mCamera->screenToRay(center);

    for (const auto& obb : mEnemyColliders) {
        if (Intersect::intersectRayOBB(ray, obb->getOBB())) {
            obb->getComponent<HitPoint>()->takeDamage(10);
            Debug::log("Hit");
            return;
        }
    }
}

void BulletShooter::setEnemy(const GameObject& enemy) {
    const auto& e = enemy.getGameObjectManager().find("Enemy");
    mEnemyColliders = e->componentManager().getComponents<OBBCollider>();
}
