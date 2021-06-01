#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class PlayerWeapon;
class Stamina;
class Time;

class PlayerAttack
    : public Component
{
public:
    PlayerAttack();
    ~PlayerAttack();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void originalUpdate();
    //攻撃動作中か
    //isAttacking & isAttackEnding
    bool isAttackOperating() const;
    //攻撃中か
    bool isAttacking() const;
    //攻撃終了中か
    bool isAttackEnding() const;

private:
    PlayerAttack(const PlayerAttack&) = delete;
    PlayerAttack& operator=(const PlayerAttack&) = delete;

    //モーション時間設定
    void setMotionTime(unsigned index, unsigned motionNo);
    //モーション変更
    void changeMotion(unsigned motionNo, bool isLoop);
    //タイマー変更
    void changeLimitTimer(float time);

    //1回目の攻撃開始
    void firstAttack();
    //2回目の攻撃開始
    void secondAttack();
    //攻撃終了
    void endAttack();
    //攻撃動作終了
    void endAttackOperation();
    //1回目の攻撃終了
    void firstAttackEnd();
    //2回目の攻撃終了
    void secondAttackEnd();
    //1回目の攻撃が可能か
    bool canFirstAttack() const;
    //2回目の攻撃が可能か
    bool canSecondAttack() const;
    //2回目から1回目に戻る攻撃が可能か
    bool canSecondToFirstAttack() const;
    //1回目の攻撃が終わるか
    bool isEndFirstAttack() const;
    //2回目の攻撃が終わるか
    bool isEndSecondAttack() const;

private:
    static constexpr unsigned FIRST_ATTACK_START_NO = 0;
    static constexpr unsigned SECOND_ATTACK_START_NO = 1;
    static constexpr unsigned FIRST_ATTACK_END_NO = 2;
    static constexpr unsigned SECOND_ATTACK_END_NO = 3;
    static constexpr unsigned NUM_ATTACK_MOTION = 2;
    static constexpr unsigned NUM_ATTACK_MOTION_PLUS_END = NUM_ATTACK_MOTION * 2;

    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<Stamina> mStamina;
    std::shared_ptr<PlayerWeapon> mWeapon;
    //モーション経過時間
    std::unique_ptr<Time> mMotionTimer;
    //各攻撃モーション時間
    float mAttackMotionTime[NUM_ATTACK_MOTION_PLUS_END];
    //追加攻撃までの最低クールタイム
    float mCoolTimeUntilAdditionalAttack[NUM_ATTACK_MOTION];
    //攻撃の消費スタミナ量
    float mAttackStaminaAmount;
};
