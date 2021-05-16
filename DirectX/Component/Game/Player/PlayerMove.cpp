#include "PlayerMove.h"
#include "PlayerDash.h"
#include "PlayerMotions.h"
#include "PlayerWalk.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Subject.h"
#include "../../../Device/Time.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"

PlayerMove::PlayerMove()
    : Component()
    , mCamera(nullptr)
    , mAnimation(nullptr)
    , mWalk(nullptr)
    , mDash(nullptr)
    , mCallbackToStop(std::make_unique<Subject>())
    , mMoveDirectionInputedLast()
{
}

PlayerMove::~PlayerMove() = default;

void PlayerMove::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    const auto& camCompManager = cam->componentManager();
    mCamera = camCompManager.getComponent<Camera>();

    mAnimation = getComponent<SkinMeshComponent>();
    mWalk = getComponent<PlayerWalk>();
    mDash = getComponent<PlayerDash>();
}

const Vector3& PlayerMove::getMoveDirectionInputedLast() const {
    return mMoveDirectionInputedLast;
}

void PlayerMove::originalUpdate() {
    const auto& leftStick = Input::joyPad().leftStick();
    if (canMove(leftStick)) {
        //スティック入力から移動方向を求める
        calcMoveDirection(leftStick);

        //ダッシュできるならダッシュする
        //無理なら歩行
        if (mDash->canDash()) {
            mDash->dash(*this);
        } else {
            mWalk->walk(*this);
        }
    } else {
        stop();
    }
}

bool PlayerMove::isMoving() const {
    if (isWalking()) {
        return true;
    }
    if (isDashing()) {
        return true;
    }

    return false;
}

bool PlayerMove::isWalking() const {
    return mWalk->isWalking();
}

bool PlayerMove::isDashing() const {
    return mDash->isDashing();
}

void PlayerMove::callbackToStop(const std::function<void()>& callback) {
    mCallbackToStop->addObserver(callback);
}

void PlayerMove::move(float moveSpeed) {
    transform().translate(mMoveDirectionInputedLast * moveSpeed * Time::deltaTime);
}

void PlayerMove::rotateToMoveDirection() {
    transform().setRotation(Quaternion::lookRotation(mMoveDirectionInputedLast));
}

void PlayerMove::calcMoveDirection(const Vector2& leftStickValue) {
    //カメラの視線ベクトルから向きを得る
    auto eye = Vector3::normalize(mCamera->getLookAt() - mCamera->getPosition());
    auto lookRot = Quaternion::lookRotation(eye);
    //カメラ向きから右、前方ベクトルを求める
    auto camRight = Vector3::transform(Vector3::right, lookRot);
    camRight.y = 0.f;
    auto camForward = Vector3::transform(Vector3::forward, lookRot);
    camForward.y = 0.f;

    //移動方向を求める
    mMoveDirectionInputedLast = camRight * leftStickValue.x + camForward * leftStickValue.y;
}

void PlayerMove::stop() {
    if (isMoving()) {
        mAnimation->changeMotion(PlayerMotions::IDOL);
        mAnimation->setLoop(true);

        mCallbackToStop->notify();
    }
}

bool PlayerMove::canMove(const Vector2& leftStickValue) const {
    return !(Vector2::equal(leftStickValue, Vector2::zero));
}
