﻿#include "BulletShooter.h"
#include "../Enemy/OctopusFoot.h"
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
    , mIsADS(false)
{
}

BulletShooter::~BulletShooter() = default;

void BulletShooter::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();
}

void BulletShooter::update() {
    ads();
    if (!mIsADS) {
        return;
    }
    if (!Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        return;
    }

    const auto center = Window::size() / 2.f;
    const auto ray = mCamera->screenToRay(center);

    for (const auto& foot : mEnemyFoots) {
        const auto& colliders = foot->getColliders();
        for (const auto& c : colliders) {
            if (!c->getEnable()) {
                continue;
            }

            if (Intersect::intersectRayOBB(ray, c->getOBB())) {
                foot->takeDamage();
                Debug::log("Hit");
                return;
            }
        }
    }
}

void BulletShooter::setEnemy(const GameObject& enemy) {
    const auto& e = enemy.getGameObjectManager().find("Enemy");
    mEnemyFoots = e->componentManager().getComponents<OctopusFoot>();
}

void BulletShooter::ads() {
    const auto& pad = Input::joyPad();
    static constexpr auto code = JoyCode::LeftButton;
    if (pad.getJoyDown(code)) {
        mIsADS = true;
        mOnStartAds();
    }
    if (pad.getJoyUp(code)) {
        mIsADS = false;
        mOnStopAds();
    }
    if (pad.getJoy(code)) {
        mOnAds();
    }
}

bool BulletShooter::isAds() const {
    return mIsADS;
}

void BulletShooter::onAds(const std::function<void()>& f) {
    mOnAds += f;
}

void BulletShooter::onStartAds(const std::function<void()>& f) {
    mOnStartAds += f;
}

void BulletShooter::onStopAds(const std::function<void()>& f) {
    mOnStopAds += f;
}