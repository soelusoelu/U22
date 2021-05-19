#include "PlayerWalk.h"
#include "PlayerDash.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

PlayerWalk::PlayerWalk()
    : Component()
    , mAnimation(nullptr)
    , mLockOn(nullptr)
    , mWalkSpeed(0.f)
    , mIsWalking(false)
{
}

PlayerWalk::~PlayerWalk() = default;

void PlayerWalk::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackChangeMotion([&] { onChangeMotion(); });

    getComponent<PlayerDash>()->callbackToDash([&] { mIsWalking = false; });
    getComponent<PlayerMove>()->callbackToStop([&] { mIsWalking = false; });
}

void PlayerWalk::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "walkSpeed", &mWalkSpeed);
}

void PlayerWalk::walk(IPlayerMove& playerMove) {
    playerMove.move(mWalkSpeed);
    rotate(playerMove);

    if (!mIsWalking) {
        mAnimation->changeMotion(PlayerMotions::WALK);
        mAnimation->setLoop(true);
        mIsWalking = true;

        mCallbackToWalk();
    }
}

bool PlayerWalk::isWalking() const {
    return mIsWalking;
}

void PlayerWalk::setILockOn(const ILockOn* lockOn) {
    mLockOn = lockOn;
}

void PlayerWalk::callbackToWalk(const std::function<void()>& callback) {
    mCallbackToWalk += callback;
}

void PlayerWalk::rotate(IPlayerMove& playerMove) {
    if (mLockOn->isLockOn()) {
        transform().lookAt(mLockOn->getLockOnTargetTransform());
    } else {
        playerMove.rotateToMoveDirection();
    }
}

void PlayerWalk::onChangeMotion() {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::WALK) {
        mIsWalking = false;
    }
}
