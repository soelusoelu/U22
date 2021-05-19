#pragma once

#include "../../Component.h"
#include "../../../Device/Subject.h"
#include <functional>
#include <memory>

class Time;

class Stamina
    : public Component
{
public:
    Stamina();
    ~Stamina();
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawInspector() override;
    //スタミナをamountに応じて使用する
    //スタミナが1でもあれば消費量に関わらず使用する
    //使用したらtrue、使用できないならfalse
    bool use(float amount);
    //スタミナ回復率を設定する
    void setHealRate(float rate);
    //スタミナ回復率をデフォルトに戻す
    void setHealRateToDefault();
    //スタミナを回復するか設定する
    void setHealFlag(bool value);
    //現在のスタミナを取得する
    float getStamina() const;
    //現在のスタミナの残量割合を取得する
    float getStaminaRate() const;
    //スタミナ変更時のコールバック
    void callbackChangeStamina(const std::function<void(const Stamina&)>& callback);

private:
    Stamina(const Stamina&) = delete;
    Stamina& operator=(const Stamina&) = delete;

    bool canUse() const;
    bool canHeal() const;

private:
    //現在のスタミナ
    float mCurrentStamina;
    //最大スタミナ
    float mMaxStamina;
    //スタミナ回復量
    float mHealAmount;
    //このフレームでスタミナを使用したか
    bool mUsingStaminaThisFrame;
    //スタミナを回復するか
    bool mHealFlag;
    //スタミナ回復量の割合
    float mHealRate;
    //スタミナが尽きてから回復し出すまでの時間
    std::unique_ptr<Time> mCoolTime;
    //スタミナ消費時のコールバック
    Subject<const Stamina&> mCallbackChangeStamina;

    static constexpr float DEFAULT_HEAL_RATE = 1.f;
};
