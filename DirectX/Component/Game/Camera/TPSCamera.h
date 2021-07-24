#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class Camera;
class GameObject;

class TPSCamera
    : public Component
{
public:
    TPSCamera();
    ~TPSCamera();
    TPSCamera(const TPSCamera&) = delete;
    TPSCamera& operator=(const TPSCamera&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    void setPlayer(const std::shared_ptr<GameObject>& player);

private:
    void calcLookAt() const;
    void calcPosition(const Vector2& mouseMoveAmount) const;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<GameObject> mPlayer;
    //カメラ回転速度
    float mRotateSpeed;
    //プレイヤーまでの距離
    float mToPlayerDistance;
    //注視点のY軸オフセット
    float mLookAtOffsetY;
    //開始時のカメラY軸位置
    float mStartPositionY;
};
