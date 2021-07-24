#include "TPSCamera.h"
#include "../../Engine/Camera/Camera.h"
#include "../../../Device/Time.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

TPSCamera::TPSCamera()
    : Component()
    , mCamera(nullptr)
    , mPlayer(nullptr)
    , mRotateSpeed(0.f)
    , mToPlayerDistance(0.f)
    , mLookAtOffsetY(0.f)
    , mStartPositionY(0.f)
{
}

TPSCamera::~TPSCamera() = default;

void TPSCamera::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();
}

void TPSCamera::update() {
    const auto& mouse = Input::mouse();
    const auto& moveAmount = mouse.getMouseMoveAmount();

    //マウスの動きが無ければ終了
    if (Vector2::equal(moveAmount, Vector2::zero)) {
        return;
    }

    calcLookAt();
    calcPosition(moveAmount);
}

void TPSCamera::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSetFloat(mRotateSpeed, "rotateSpeed", inObj, alloc, mode);
    JsonHelper::getSetFloat(mToPlayerDistance, "cameraToPlayerDistance", inObj, alloc, mode);
    JsonHelper::getSetFloat(mLookAtOffsetY, "lookAtOffsetY", inObj, alloc, mode);
    JsonHelper::getSetFloat(mStartPositionY, "startPositionY", inObj, alloc, mode);
}

void TPSCamera::setPlayer(const std::shared_ptr<GameObject>& player) {
    mPlayer = player;

    const auto& pt = mPlayer->transform();
    const auto& pp = pt.getPosition();

    auto lookAt = pp + Vector3::up * mLookAtOffsetY;
    auto pos = pp + -pt.forward() * mToPlayerDistance + Vector3::up * mStartPositionY;

    mCamera->lookAt(lookAt);
    mCamera->setPosition(pos);
}

void TPSCamera::calcLookAt() const {
    const auto& pt = mPlayer->transform();
    auto playerUpPoint = pt.getPosition() + Vector3::up * mLookAtOffsetY * pt.getScale().y;
    mCamera->lookAt(playerUpPoint);
}

void TPSCamera::calcPosition(const Vector2& mouseMoveAmount) const {
    auto& t = transform();
    auto euler = t.getRotation().euler();
    euler.y += mouseMoveAmount.x * mRotateSpeed * Time::deltaTime;
    euler.x += mouseMoveAmount.y * mRotateSpeed * Time::deltaTime;
    t.setRotation(Quaternion(euler));
    auto cameraPos = mCamera->getLookAt() + -t.forward() * mToPlayerDistance;
    mCamera->setPosition(cameraPos);
}
