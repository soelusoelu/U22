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

    //歩行時に行う処理
    void walk(const Vector2& leftStickValue);
    //ダッシュ時に行う処理
    void dash(const JoyPad& pad, const Vector2& leftStickValue);
    //移動・回転処理
    void move(float moveSpeed, const Vector2& leftStickValue);
    //移動停止処理
    void stop();
    //動ける状態か
    bool canMove(const Vector2& leftStickValue) const;
    //ダッシュできる状態か
    bool canDash(const JoyPad& pad);
    //モーション変更時のコールバック
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
