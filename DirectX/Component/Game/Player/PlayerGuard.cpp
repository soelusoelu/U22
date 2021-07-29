#include "PlayerGuard.h"
#include "PlayerMotions.h"
#include "Stamina.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Utility/JsonHelper.h"

PlayerGuard::PlayerGuard()
    : Component()
    , mAnimation(nullptr)
    , mStamina(nullptr)
    , mIsGuarding(false)
    , mGuardingStaminaHealRate(0.f)
{
}

PlayerGuard::~PlayerGuard() = default;

void PlayerGuard::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mStamina = getComponent<Stamina>();
}

void PlayerGuard::update() {
    if (!mIsGuarding) {
        return;
    }

    const auto& pad = Input::joyPad();
    if (pad.getJoyUp(GUARD_BUTTON)) {
        mAnimation->changeMotion(PlayerMotions::GUARD_END);
        mAnimation->setLoop(false);
        mStamina->setHealRateToDefault();
        mIsGuarding = false;
    }
}

void PlayerGuard::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mGuardingStaminaHealRate, "guardingStaminaHealRate", inObj, alloc, mode);
}

void PlayerGuard::originalUpdate() {
    if (Input::joyPad().getJoyDown(GUARD_BUTTON)) {
        mAnimation->changeMotion(PlayerMotions::GUARD_START);
        mAnimation->setLoop(false);
        mStamina->setHealRate(mGuardingStaminaHealRate);
        mIsGuarding = true;
    }
}

bool PlayerGuard::isGuarding() const {
    return mIsGuarding;
}
