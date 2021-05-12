#include "Stamina.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Math/Math.h"
#include "../../../Utility/LevelLoader.h"

Stamina::Stamina()
    : Component()
    , mCurrentStamina(0)
    , mMaxStamina(0)
    , mUsingStamina(false)
    , mPreviousUsed(false)
    , mCoolTime(std::make_unique<Time>())
{
}

Stamina::~Stamina() = default;

void Stamina::lateUpdate() {
    if (canHeal()) {
        ++mCurrentStamina;
    }

    mPreviousUsed = mUsingStamina;
    mUsingStamina = false;
}

void Stamina::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "stamina", &mCurrentStamina);
    mMaxStamina = mCurrentStamina;
    if (float time = 0.f; JsonHelper::getFloat(inObj, "coolTime", &time)) {
        mCoolTime->setLimitTime(time);
    }
}

void Stamina::drawInspector() {
    ImGuiWrapper::sliderInt("stamina", mCurrentStamina, 0, mMaxStamina);
}

bool Stamina::use(int amount) {
    if (!canUse()) {
        return false;
    }

    mCurrentStamina -= amount;
    if (mCurrentStamina <= 0) {
        mCurrentStamina = 0;

        mCoolTime->reset();
    }

    mUsingStamina = true;

    return true;
}

bool Stamina::canUse() const {
    return (mCurrentStamina > 0);
}

bool Stamina::canHeal() const {
    if (!mCoolTime->isTime()) {
        mCoolTime->update();
        return false;
    }
    if (mUsingStamina) {
        return false;
    }
    if (mCurrentStamina == mMaxStamina) {
        return false;
    }

    return true;
}
