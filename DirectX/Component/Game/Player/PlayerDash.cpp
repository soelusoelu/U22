#include "PlayerDash.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "PlayerWalk.h"
#include "Stamina.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

PlayerDash::PlayerDash()
    : Component()
    , mAnimation(nullptr)
    , mStamina(nullptr)
    , mDashMigrationTimer(std::make_unique<Time>())
    , mDashSpeed(0.f)
    , mIsDashing(false)
    , mShouldReleaseDashButton(false)
    , mDashStaminaAmount(0.f)
{
}

PlayerDash::~PlayerDash() = default;

void PlayerDash::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackChangeMotion([&] { onChangeMotion(); });
    mStamina = getComponent<Stamina>();

    getComponent<PlayerWalk>()->callbackToWalk([&] { mIsDashing = false; });
    getComponent<PlayerMove>()->callbackToStop([&] { mIsDashing = false; });
}

void PlayerDash::lateUpdate() {
    if (!mShouldReleaseDashButton) {
        return;
    }
    if (Input::joyPad().getJoyUp(DASH_BUTTON)) {
        mShouldReleaseDashButton = false;
    }
}

void PlayerDash::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSetFloat(mDashSpeed, "dashSpeed", inObj, alloc, mode);
    JsonHelper::getSetFloat(mDashStaminaAmount, "dashStaminaAmount", inObj, alloc, mode);
    mDashMigrationTimer->saveAndLoad(inObj, alloc, mode);
}

void PlayerDash::dash(IPlayerMove& playerMove) {
    playerMove.move(mDashSpeed);
    playerMove.rotateToMoveDirection();

    if (!mIsDashing) {
        mAnimation->changeMotion(PlayerMotions::DASH);
        mIsDashing = true;

        mCallbackToDash();
    }
}

bool PlayerDash::isDashing() const {
    return mIsDashing;
}

bool PlayerDash::canDash() {
    //ダッシュボタンを離すまで終了
    if (mShouldReleaseDashButton) {
        return false;
    }

    //ダッシュボタンが押されていないなら終了
    if (!Input::joyPad().getJoy(DASH_BUTTON)) {
        mDashMigrationTimer->reset();
        return false;
    }

    //ダッシュボタンを一定時間押していなければ終了
    if (!mDashMigrationTimer->isTime()) {
        mDashMigrationTimer->update();
        return false;
    }

    //スタミナが0なら終了
    if (!mStamina->use(mDashStaminaAmount)) {
        mShouldReleaseDashButton = true;
        mCallbackRunOutOfStamina();
        return false;
    }

    return true;
}

void PlayerDash::callbackToDash(const std::function<void()>& callback) {
    mCallbackToDash += callback;
}

void PlayerDash::callbackRunOutOfStamina(const std::function<void()>& callback) {
    mCallbackRunOutOfStamina += callback;
}

void PlayerDash::onChangeMotion() {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::DASH) {
        mIsDashing = false;
    }
}
