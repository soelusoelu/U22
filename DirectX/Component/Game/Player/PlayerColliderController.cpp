#include "PlayerColliderController.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../../Engine/Collider/Collider.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"

PlayerColliderController::PlayerColliderController()
    : Component()
    , mHP(nullptr)
    , mIsHitThisFrame(false)
    , mIsPreviousHit(false)
{
}

PlayerColliderController::~PlayerColliderController() = default;

void PlayerColliderController::start() {
    mHP = getComponent<HitPoint>();
}

void PlayerColliderController::update() {
    mIsPreviousHit = mIsHitThisFrame;
    mIsHitThisFrame = false;
}

void PlayerColliderController::onCollisionEnter(Collider& other) {
    //1フレームに1回しかヒットさせない
    if (mIsHitThisFrame) {
        return;
    }
    //前フレームにヒットしていたら終了
    if (mIsPreviousHit) {
        return;
    }

    if (other.gameObject().tag() == "Enemy") {
        mHP->takeDamage(10);

        mIsHitThisFrame = true;

        Debug::log("damege!");

        return;
    }
}
