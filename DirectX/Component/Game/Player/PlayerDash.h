#pragma once

#include "IPlayerMove.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include "../../../Input/Input.h"
#include <functional>
#include <memory>

class SkinMeshComponent;
class Stamina;
class Time;

class PlayerDash
    : public Component
{
public:
    PlayerDash();
    ~PlayerDash();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    //ダッシュ処理
    void dash(IPlayerMove& playerMove);
    //走っているか
    bool isDashing() const;
    //ダッシュできるか
    bool canDash();
    //歩行に移行した際のコールバック
    void callbackToDash(const std::function<void()>& callback);
    //ダッシュでスタミナが尽きた際のコールバック
    void callbackRunOutOfStamina(const std::function<void()>& callback);

private:
    PlayerDash(const PlayerDash&) = delete;
    PlayerDash& operator=(const PlayerDash&) = delete;

    //モーション変更時のコールバック
    void onChangeMotion();

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<Stamina> mStamina;
    std::unique_ptr<Time> mDashMigrationTimer;
    Function<void()> mCallbackToDash;
    Function<void()> mCallbackRunOutOfStamina;
    //ダッシュ速度
    float mDashSpeed;
    //ダッシュ中か
    bool mIsDashing;
    //ダッシュボタンを離すべきか
    bool mShouldReleaseDashButton;
    //ダッシュの使用スタミナ量
    float mDashStaminaAmount;

    static constexpr JoyCode DASH_BUTTON = JoyCode::B;
};
