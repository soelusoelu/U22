#pragma once

#include "../../Component.h"
#include "../../../Input/Input.h"
#include "../../../Math/Math.h"
#include <functional>
#include <memory>

class Camera;
class SkinMeshComponent;
class Stamina;
class Time;
class Subject;

class PlayerMove
    : public Component
{
    enum class State {
        IDOL,
        WALK,
        DASH
    };

public:
    PlayerMove();
    ~PlayerMove();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void originalUpdate();
    //動いているか
    bool isMoving() const;
    //歩いているか
    bool isWalking() const;
    //走っているか
    bool isDashing() const;
    //ダッシュでスタミナが尽きた際のコールバック
    void callbackRunOutOfStamina(const std::function<void()>& callback);

private:
    PlayerMove(const PlayerMove&) = delete;
    PlayerMove& operator=(const PlayerMove&) = delete;

    void walk(const Vector2& leftStickValue);
    void dash(const JoyPad& pad, const Vector2& leftStickValue);
    void move(float moveSpeed, const Vector2& leftStickValue);
    void stop();
    bool canMove(const Vector2& leftStickValue) const;
    bool canDash(const JoyPad& pad);
    void onChangeMotion();

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<Stamina> mStamina;
    std::unique_ptr<Time> mDashMigrationTimer;
    std::unique_ptr<Subject> mCallbackRunOutOfStamina;
    float mWalkSpeed;
    float mDashSpeed;
    bool mIsWalking;
    bool mIsDashing;
    bool mShouldReleaseDashButton;
    int mDashStaminaAmount;
    static constexpr JoyCode DASH_BUTTON = JoyCode::B;
};
