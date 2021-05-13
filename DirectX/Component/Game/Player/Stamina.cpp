#include "Stamina.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Math/Math.h"
#include "../../../Utility/LevelLoader.h"

Stamina::Stamina()
    : Component()
    , mCurrentStamina(0.f)
    , mMaxStamina(0.f)
    , mHealAmount(0.f)
    , mUsingStaminaThisFrame(false)
    , mHealFlag(true)
    , mHealRate(DEFAULT_HEAL_RATE)
    , mCoolTime(std::make_unique<Time>())
{
}

Stamina::~Stamina() = default;

void Stamina::lateUpdate() {
    if (canHeal()) {
        mCurrentStamina += mHealAmount * mHealRate * Time::deltaTime;
        mCurrentStamina = Math::Min(mCurrentStamina, mMaxStamina);
    }

    if (mUsingStaminaThisFrame) {
        mUsingStaminaThisFrame = false;
    }
}

void Stamina::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "stamina", &mCurrentStamina);
    mMaxStamina = mCurrentStamina;
    JsonHelper::getFloat(inObj, "healAmount", &mHealAmount);
    if (float time = 0.f; JsonHelper::getFloat(inObj, "coolTime", &time)) {
        mCoolTime->setLimitTime(time);
    }
}

void Stamina::drawInspector() {
    ImGuiWrapper::sliderFloat("stamina", mCurrentStamina, 0.f, mMaxStamina);
}

bool Stamina::use(float amount) {
    if (!canUse()) {
        return false;
    }

    mCurrentStamina -= amount;
    if (mCurrentStamina <= 0.f) {
        mCurrentStamina = 0.f;

        mCoolTime->reset();
    }

    mUsingStaminaThisFrame = true;

    return true;
}

void Stamina::setHealRate(float rate) {
    mHealRate = rate;
}

void Stamina::setHealRateToDefault() {
    mHealRate = DEFAULT_HEAL_RATE;
}

void Stamina::setHealFlag(bool value) {
    mHealFlag = value;
}

bool Stamina::canUse() const {
    return (mCurrentStamina > 0.f);
}

bool Stamina::canHeal() const {
    if (!mHealFlag) {
        return false;
    }
    if (!mCoolTime->isTime()) {
        mCoolTime->update();
        return false;
    }
    if (mUsingStaminaThisFrame) {
        return false;
    }
    if (Math::equal(mCurrentStamina, mMaxStamina)) {
        return false;
    }

    return true;
}
