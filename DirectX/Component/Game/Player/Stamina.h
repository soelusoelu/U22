#pragma once

#include "../../Component.h"
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
    bool use(int amount);
    //スタミナを回復するか設定する
    void setHealFlag(bool value);

private:
    Stamina(const Stamina&) = delete;
    Stamina& operator=(const Stamina&) = delete;

    bool canUse() const;
    bool canHeal() const;

private:
    int mCurrentStamina;
    int mMaxStamina;
    bool mUsingStamina;
    bool mPreviousUsed;
    bool mHealFlag;
    std::unique_ptr<Time> mCoolTime;
};
