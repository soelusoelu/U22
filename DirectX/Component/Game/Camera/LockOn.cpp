#include "LockOn.h"
#include "GameCamera.h"
#include "../../Engine/Camera/Camera.h"
#include "../../../Device/Subject.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

LockOn::LockOn()
    : Component()
    , mCamera(nullptr)
    , mGameCamera(nullptr)
    , mPlayer(nullptr)
    , mLockOnTarget(nullptr)
    , mCallbackLockOn(std::make_unique<Subject>())
    , mLockOnAngle(0.f)
    , mIsLockOn(false)
    , mLookAtOffsetY(0.f)
    , mCameraOffsetY(0.f)
{
}

LockOn::~LockOn() = default;

void LockOn::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    mGameCamera = getComponent<GameCamera>();
}

void LockOn::update() {
    if (!mIsLockOn) {
        lockOn();
    }
}

void LockOn::lateUpdate() {
    if (mIsLockOn) {
        lockOnUpdate();
    }
}

void LockOn::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "lockOnAngle", &mLockOnAngle);
    JsonHelper::getFloat(inObj, "lookAtOffsetY", &mLookAtOffsetY);
    JsonHelper::getFloat(inObj, "cameraOffsetY", &mCameraOffsetY);
}

bool LockOn::isLockOn() const {
    return mIsLockOn;
}

void LockOn::callbackLockOn(const std::function<void()>& callback) {
    mCallbackLockOn->addObserver(callback);
}

void LockOn::setPlayer(const Player& player) {
    mPlayer = player;
}

void LockOn::setEnemys(const Enemys& enemys) {
    mEnemys = enemys;
}

void LockOn::lockOn() {
    if (!Input::joyPad().getJoyDown(JoyCode::RightStickButton)) {
        return;
    }

    for (const auto& e : mEnemys) {
        const auto& ePos = e->transform().getPosition();
        mIsLockOn = isLockOnRange(ePos);
        if (mIsLockOn) {
            mLockOnTarget = e;

            mCallbackLockOn->notify();
            return;
        }
    }
}

void LockOn::lockOnUpdate() {
    auto lookAt = mLockOnTarget->transform().getPosition() + Vector3::up * mLookAtOffsetY;
    mCamera->lookAt(lookAt);

    const auto& pt = mPlayer->transform();
    auto playerToLookAt = Vector3::normalize(lookAt - pt.getPosition());
    playerToLookAt.y = 0.f;
    mCamera->setPosition(pt.getPosition() + -playerToLookAt * mGameCamera->getDistanceToPlayer() + Vector3::up * mCameraOffsetY);
}

bool LockOn::isLockOnRange(const Vector3& enemyPosition) const {
    //プレイヤーから見たエネミーを-1から1に換算する
    const auto& pt = mPlayer->transform();
    auto p2e = Vector3::normalize(enemyPosition - pt.getPosition());
    auto dot = Vector3::dot(pt.forward(), p2e);
    //角度に変換する
    auto angle = Math::acos(dot);

    return (angle < mLockOnAngle);
}
