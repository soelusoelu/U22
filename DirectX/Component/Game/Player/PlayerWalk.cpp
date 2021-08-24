#include "PlayerWalk.h"
#include "BulletShooter.h"
#include "PlayerDash.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "PlayerSounds.h"
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

PlayerWalk::PlayerWalk()
    : Component()
    , mAnimation(nullptr)
    , mSound(nullptr)
    , mBulletShooter(nullptr)
    , mCameraRotation(nullptr)
    , mWalkSpeed(0.f)
    , mIsWalking(false)
{
}

PlayerWalk::~PlayerWalk() = default;

void PlayerWalk::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackChangeMotion([&] { onChangeMotion(); });
    mBulletShooter = getComponent<BulletShooter>();
    mBulletShooter->onAds([&] { onAds(); });

    mSound = getComponents<SoundComponent>()[PlayerSounds::DASH];
    //音を高くするために
    mSound->getSoundPlayer().getFrequency().setFrequencyRatio(2.f);
    //音の反響を無くすため
    mSound->getSoundEmitter().setCalculateReverb(false);

    getComponent<PlayerDash>()->callbackToDash([&] { mIsWalking = false; });
    getComponent<PlayerMove>()->callbackToStop([&] { mIsWalking = false; mSound->getSoundPlayer().stop(); });
}

void PlayerWalk::onCollisionEnter(Collider& other) {
    if (other.gameObject().tag() == "Enemy") {
        mSound->getSoundPlayer().stop();
    }
}

void PlayerWalk::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mWalkSpeed, "walkSpeed", inObj, alloc, mode);
}

void PlayerWalk::walk(IPlayerMove& playerMove) {
    playerMove.move(mWalkSpeed);
    //rotate(playerMove);

    if (!mIsWalking) {
        //mAnimation->changeMotion(PlayerMotions::LEFT_MOVE_ON_ADS);
        //mAnimation->setLoop(true);
        mIsWalking = true;

        auto& soundPlayer = mSound->getSoundPlayer();
        soundPlayer.setPlayPoint(0.f);
        soundPlayer.playStreaming();
        soundPlayer.getLoop().loopAll();
        //音を遅く・低くするために
        soundPlayer.getFrequency().setFrequencyRatio(1.5f);

        mCallbackToWalk();
    }
}

bool PlayerWalk::isWalking() const {
    return mIsWalking;
}

void PlayerWalk::setCameraRotation(const Quaternion& cameraRotation) {
    mCameraRotation = &cameraRotation;
}

void PlayerWalk::callbackToWalk(const std::function<void()>& callback) {
    mCallbackToWalk += callback;
}

void PlayerWalk::rotate(IPlayerMove& playerMove) {
    if (!mBulletShooter->isAds()) {
        playerMove.rotateToMoveDirection();
    }
}

void PlayerWalk::onChangeMotion() {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::LEFT_MOVE_ON_ADS) {
        mIsWalking = false;
    }
}

void PlayerWalk::onAds() {
    //カメラ角度のY軸だけ考慮して回転する
    auto euler = mCameraRotation->euler();
    euler.x = 0.f;
    euler.z = 0.f;
    transform().setRotation(euler);
}
