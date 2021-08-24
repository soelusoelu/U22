#include "PlayerDash.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "PlayerSounds.h"
#include "PlayerWalk.h"
#include "../../Engine/Collider/Collider.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../Engine/Sound/SoundComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Sound/3D/Emitter/Sound3DEmitter.h"
#include "../../../Sound/Player/Frequency.h"
#include "../../../Sound/Player/SoundLoop.h"
#include "../../../Sound/Player/SoundPlayer.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

PlayerDash::PlayerDash()
    : Component()
    , mAnimation(nullptr)
    , mSound(nullptr)
    , mDashSpeed(0.f)
    , mIsDashing(false)
{
}

PlayerDash::~PlayerDash() = default;

void PlayerDash::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackChangeMotion([&] { onChangeMotion(); });

    mSound = getComponents<SoundComponent>()[PlayerSounds::DASH];
    //音の反響を無くすため
    mSound->getSoundEmitter().setCalculateReverb(false);

    getComponent<PlayerWalk>()->callbackToWalk([&] { mIsDashing = false; });
    getComponent<PlayerMove>()->callbackToStop([&] { mIsDashing = false; mSound->getSoundPlayer().stop(); });
}

void PlayerDash::onCollisionEnter(Collider& other) {
    if (other.gameObject().tag() == "Enemy") {
        mSound->getSoundPlayer().stop();
    }
}

void PlayerDash::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mDashSpeed, "dashSpeed", inObj, alloc, mode);
}

void PlayerDash::dash(IPlayerMove& playerMove) {
    playerMove.move(mDashSpeed);
    playerMove.rotateToMoveDirection();

    if (!mIsDashing) {
        mAnimation->changeMotion(PlayerMotions::DASH);
        mIsDashing = true;

        auto& soundPlayer = mSound->getSoundPlayer();
        soundPlayer.setPlayPoint(0.f);
        soundPlayer.playStreaming();
        soundPlayer.getLoop().loopAll();
        //音を速く・高くするために
        soundPlayer.getFrequency().setFrequencyRatio(2.f);

        mCallbackToDash();
    }
}

bool PlayerDash::isDashing() const {
    return mIsDashing;
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
