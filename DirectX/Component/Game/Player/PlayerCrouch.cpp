#include "PlayerCrouch.h"
#include "PlayerMotions.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"

PlayerCrouch::PlayerCrouch()
    : Component()
    , mAnimation(nullptr)
    , mCrouchTimer(std::make_unique<Time>())
{
}

PlayerCrouch::~PlayerCrouch() = default;

void PlayerCrouch::start() {
    mAnimation = getComponent<SkinMeshComponent>();

    const auto& crouch = mAnimation->getMotion(PlayerMotions::CROUCH);
    auto motionTime = static_cast<float>(crouch.numFrame) / 60.f;

    //しゃがみモーションを行う時間をファイルから設定された時間 + モーションそのものの時間にする
    mCrouchTimer->setLimitTime(mCrouchTimer->getLimitTime() + motionTime);
}

void PlayerCrouch::update() {
    if (isCrouching()) {
        updateCrouch();
    }
    if (isStandingUp()) {
        updateStandUp();
    }
}

void PlayerCrouch::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mCrouchTimer->saveAndLoad(inObj, alloc, mode);
}

void PlayerCrouch::originalUpdate() {
    if (isCrouching()) {
        return;
    }
    if (isStandingUp()) {
        return;
    }
    if (!Input::joyPad().getJoyDown(JoyCode::A)) {
        return;
    }

    crouch();
}

bool PlayerCrouch::isCrouching() const {
    return (mAnimation->getCurrentMotionNumber() == PlayerMotions::CROUCH);
}

bool PlayerCrouch::isStandingUp() const {
    return (mAnimation->getCurrentMotionNumber() == PlayerMotions::STAND_UP);
}

void PlayerCrouch::crouch() {
    mAnimation->changeMotion(PlayerMotions::CROUCH);
    mAnimation->setLoop(false);
}

void PlayerCrouch::updateCrouch() {
    mCrouchTimer->update();
    if (mCrouchTimer->isTime()) {
        mCrouchTimer->reset();

        standUp();
    }
}

void PlayerCrouch::standUp() {
    mAnimation->changeMotion(PlayerMotions::STAND_UP);
    mAnimation->setLoop(false);
}

void PlayerCrouch::updateStandUp() {
    auto curFrame = mAnimation->getCurrentMotionFrame();
    if (mAnimation->getCurrentMotion().numFrame - 1 == curFrame) {
        mAnimation->changeMotion(PlayerMotions::IDOL);
        mAnimation->setLoop(true);
    }
}
