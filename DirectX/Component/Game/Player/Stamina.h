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
    bool use(int amount);

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
    std::unique_ptr<Time> mCoolTime;
};
