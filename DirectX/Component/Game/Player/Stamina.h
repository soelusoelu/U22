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
    //�X�^�~�i��amount�ɉ����Ďg�p����
    //�X�^�~�i��1�ł�����Ώ���ʂɊւ�炸�g�p����
    //�g�p������true�A�g�p�ł��Ȃ��Ȃ�false
    bool use(float amount);
    //�X�^�~�i�񕜗���ݒ肷��
    void setHealRate(float rate);
    //�X�^�~�i�񕜗����f�t�H���g�ɖ߂�
    void setHealRateToDefault();
    //�X�^�~�i���񕜂��邩�ݒ肷��
    void setHealFlag(bool value);
    //���݂̃X�^�~�i���擾����
    float getStamina() const;
    //���݂̃X�^�~�i�̎c�ʊ������擾����
    float getStaminaRate() const;
    //�X�^�~�i�ύX���̃R�[���o�b�N
    void callbackChangeStamina(const std::function<void(const Stamina&)>& callback);

private:
    Stamina(const Stamina&) = delete;
    Stamina& operator=(const Stamina&) = delete;

    bool canUse() const;
    bool canHeal() const;

private:
    //���݂̃X�^�~�i
    float mCurrentStamina;
    //�ő�X�^�~�i
    float mMaxStamina;
    //�X�^�~�i�񕜗�
    float mHealAmount;
    //���̃t���[���ŃX�^�~�i���g�p������
    bool mUsingStaminaThisFrame;
    //�X�^�~�i���񕜂��邩
    bool mHealFlag;
    //�X�^�~�i�񕜗ʂ̊���
    float mHealRate;
    //�X�^�~�i���s���Ă���񕜂��o���܂ł̎���
    std::unique_ptr<Time> mCoolTime;
    //�X�^�~�i����̃R�[���o�b�N
    Subject<const Stamina&> mCallbackChangeStamina;

    static constexpr float DEFAULT_HEAL_RATE = 1.f;
};
