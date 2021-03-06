#include "PlayerAnimationController.h"
#include "BulletShooter.h"
#include "PlayerCrouch.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "../Scene/GameStartTimer.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"

PlayerAnimationController::PlayerAnimationController()
    : Component()
    , mAnimation(nullptr)
    , mMove(nullptr)
    , mCrouch(nullptr)
    , mShooter(nullptr)
    , mIsUpdate(false)
{
}

PlayerAnimationController::~PlayerAnimationController() = default;

void PlayerAnimationController::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->changeMotion(PlayerMotions::IDOL);

    mMove = getComponent<PlayerMove>();
    mCrouch = getComponent<PlayerCrouch>();
    mShooter = getComponent<BulletShooter>();

    const auto& gst = gameObject().getGameObjectManager().find("GameStartTimer");
    gst->componentManager().getComponent<GameStartTimer>()->onEndTimer([&] { mIsUpdate = true; });
}

void PlayerAnimationController::update() {
    if (!mIsUpdate) {
        return;
    }

    //死亡しているなら終了
    if (mAnimation->getCurrentMotionNumber() == PlayerMotions::DEAD) {
        return;
    }
    //被ダメージモーション中は何もできない
    if (mAnimation->getCurrentMotionNumber() == PlayerMotions::TAKE_DAMAGE) {
        return;
    }

    if (!mShooter->isAds()) {
        mCrouch->originalUpdate();
    }

    if (!mCrouch->isCrouching() && !mCrouch->isStandingUp()) {
        mMove->originalUpdate();
        mShooter->originalUpdate();
    }
}
