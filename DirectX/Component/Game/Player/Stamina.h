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
    //�X�^�~�i��amount�ɉ����Ďg�p����
    //�X�^�~�i��1�ł�����Ώ���ʂɊւ�炸�g�p����
    //�g�p������true�A�g�p�ł��Ȃ��Ȃ�false
    bool use(int amount);
    //�X�^�~�i���񕜂��邩�ݒ肷��
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
