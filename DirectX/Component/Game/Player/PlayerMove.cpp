#include "PlayerMove.h"
#include "PlayerMotions.h"
#include "Stamina.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Subject.h"
#include "../../../Device/Time.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

PlayerMove::PlayerMove()
    : Component()
    , mCamera(nullptr)
    , mAnimation(nullptr)
    , mStamina(nullptr)
    , mDashMigrationTimer(std::make_unique<Time>())
    , mCallbackRunOutOfStamina(std::make_unique<Subject>())
    , mLockOn(nullptr)
    , mWalkSpeed(0.f)
    , mDashSpeed(0.f)
    , mIsWalking(false)
    , mIsDashing(false)
    , mShouldReleaseDashButton(false)
    , mDashStaminaAmount(0.f)
{
}

PlayerMove::~PlayerMove() = default;

void PlayerMove::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    const auto& camCompManager = cam->componentManager();
    mCamera = camCompManager.getComponent<Camera>();

    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackChangeMotion([&] { onChangeMotion(); });
    mStamina = getComponent<Stamina>();
}

void PlayerMove::lateUpdate() {
    if (!mShouldReleaseDashButton) {
        return;
    }
    if (Input::joyPad().getJoyUp(DASH_BUTTON)) {
        mShouldReleaseDashButton = false;
    }
}

void PlayerMove::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "walkSpeed", &mWalkSpeed);
    JsonHelper::getFloat(inObj, "dashSpeed", &mDashSpeed);
    if (float time = 0.f; JsonHelper::getFloat(inObj, "dashMigrationTime", &time)) {
        mDashMigrationTimer->setLimitTime(time);
    }
    JsonHelper::getFloat(inObj, "dashStaminaAmount", &mDashStaminaAmount);
}

void PlayerMove::originalUpdate() {
    const auto& pad = Input::joyPad();
    const auto& leftStick = pad.leftStick();
    if (canMove(leftStick)) {
        auto moveDir = calcMoveDirection(leftStick);
        if (canDash(pad)) {
            dash(pad, moveDir);
        } else {
            walk(moveDir);
        }
    } else {
        stop();
    }
}

bool PlayerMove::isMoving() const {
    if (mIsWalking) {
        return true;
    }
    if (mIsDashing) {
        return true;
    }

    return false;
}

bool PlayerMove::isWalking() const {
    return mIsWalking;
}

bool PlayerMove::isDashing() const {
    return mIsDashing;
}

void PlayerMove::callbackRunOutOfStamina(const std::function<void()>& callback) {
    mCallbackRunOutOfStamina->addObserver(callback);
}

void PlayerMove::setILockOn(const ILockOn* lockOn) {
    mLockOn = lockOn;
}

void PlayerMove::walk(const Vector3& moveDir) {
    moveAndRotate(moveDir, mWalkSpeed);

    if (!mIsWalking) {
        mAnimation->changeMotion(PlayerMotions::WALK);
        mAnimation->setLoop(true);
        mIsWalking = true;
        mIsDashing = false;
    }
}

void PlayerMove::dash(const JoyPad& pad, const Vector3& moveDir) {
    moveAndRotate(moveDir, mDashSpeed);

    if (!mIsDashing) {
        mAnimation->changeMotion(PlayerMotions::DASH);
        mIsWalking = false;
        mIsDashing = true;
    }
}

void PlayerMove::moveAndRotate(const Vector3& moveDir, float moveSpeed) {
    move(moveDir, moveSpeed);
    rotate(moveDir);
}

void PlayerMove::move(const Vector3& moveDir, float moveSpeed) {
    transform().translate(moveDir * moveSpeed * Time::deltaTime);
}

void PlayerMove::rotate(const Vector3& moveDir) {
    auto& t = transform();
    Vector3 forward = moveDir;

    if (mLockOn->isLockOn()) {
        forward = Vector3::normalize(mLockOn->getLockOnTargetPosition() - t.getPosition());
    }

    t.setRotation(Quaternion::lookRotation(forward));
}

Vector3 PlayerMove::calcMoveDirection(const Vector2& leftStickValue) const {
    //カメラの視線ベクトルから向きを得る
    auto eye = Vector3::normalize(mCamera->getLookAt() - mCamera->getPosition());
    auto lookRot = Quaternion::lookRotation(eye);
    //カメラ向きから右、前方ベクトルを求める
    auto camRight = Vector3::transform(Vector3::right, lookRot);
    camRight.y = 0.f;
    auto camForward = Vector3::transform(Vector3::forward, lookRot);
    camForward.y = 0.f;

    //移動方向を求める
    auto moveDir = camRight * leftStickValue.x + camForward * leftStickValue.y;

    return moveDir;
}

void PlayerMove::stop() {
    if (mIsWalking || mIsDashing) {
        mAnimation->changeMotion(PlayerMotions::IDOL);
        mAnimation->setLoop(true);
        mIsWalking = false;
        mIsDashing = false;
    }
}

bool PlayerMove::canMove(const Vector2& leftStickValue) const {
    return !(Vector2::equal(leftStickValue, Vector2::zero));
}

bool PlayerMove::canDash(const JoyPad& pad) {
    //ダッシュボタンを離すまで終了
    if (mShouldReleaseDashButton) {
        return false;
    }

    //ダッシュボタンが押されていないなら終了
    if (!pad.getJoy(DASH_BUTTON)) {
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
        mCallbackRunOutOfStamina->notify();
        return false;
    }

    return true;
}

void PlayerMove::onChangeMotion() {
    auto curMotionNo = mAnimation->getCurrentMotionNumber();
    if (curMotionNo == PlayerMotions::WALK) {
        return;
    }
    if (curMotionNo == PlayerMotions::DASH) {
        return;
    }

    mIsWalking = false;
    mIsDashing = false;
}
