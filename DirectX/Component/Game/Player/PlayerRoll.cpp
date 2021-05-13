#include "PlayerRoll.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "Stamina.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

PlayerRoll::PlayerRoll()
    : Component()
    , mAnimation(nullptr)
    , mStamina(nullptr)
    , mRollingDistance(0.f)
    , mIsRolling(false)
    , mRollingMotionTime(std::make_unique<Time>())
    , mRollingStartPoint()
    , mRollingEndPoint()
    , mShouldReleaseRollingButton(false)
    , mRollingStaminaAmount(0.f)
{
}

PlayerRoll::~PlayerRoll() = default;

void PlayerRoll::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mStamina = getComponent<Stamina>();

    const auto& rollingMotion = mAnimation->getMotion(PlayerMotions::ROLL);
    auto limit = static_cast<float>(rollingMotion.numFrame) / 60.f;
    mRollingMotionTime->setLimitTime(limit);

    getComponent<PlayerMove>()->callbackRunOutOfStamina([&] {
        mShouldReleaseRollingButton = true;
    });
}

void PlayerRoll::update() {
    if (!mIsRolling) {
        return;
    }

    mRollingMotionTime->update();
    if (mRollingMotionTime->isTime()) {
        mRollingMotionTime->reset();
        mAnimation->changeMotion(PlayerMotions::IDOL);
        mStamina->setHealFlag(true);
        mIsRolling = false;
        return;
    }

    transform().setPosition(Vector3::lerp(mRollingStartPoint, mRollingEndPoint, mRollingMotionTime->rate()));
}

void PlayerRoll::lateUpdate() {
    if (!mShouldReleaseRollingButton) {
        return;
    }
    if (Input::joyPad().getJoyUp(JoyCode::B)) {
        mShouldReleaseRollingButton = false;
    }
}

void PlayerRoll::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "rollingDistance", &mRollingDistance);
    JsonHelper::getFloat(inObj, "rollingStaminaAmount", &mRollingStaminaAmount);
}

void PlayerRoll::originalUpdate() {
    if (mShouldReleaseRollingButton) {
        return;
    }
    if (!Input::joyPad().getJoyUp(JoyCode::B)) {
        return;
    }
    if (!mStamina->use(mRollingStaminaAmount)) {
        return;
    }

    mAnimation->changeMotion(PlayerMotions::ROLL);
    mAnimation->setLoop(false);
    mStamina->setHealFlag(false);
    mIsRolling = true;

    auto& t = transform();
    mRollingStartPoint = t.getPosition();
    mRollingEndPoint = t.getPosition() + t.forward() * mRollingDistance;
}

bool PlayerRoll::isRolling() const {
    return mIsRolling;
}
