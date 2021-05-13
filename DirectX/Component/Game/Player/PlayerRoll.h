#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class SkinMeshComponent;
class Stamina;
class Time;

class PlayerRoll
    : public Component
{
public:
    PlayerRoll();
    ~PlayerRoll();
    virtual void start() override;
    virtual void update() override;
    virtual void lateUpdate() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void originalUpdate();
    //ローリング中か
    bool isRolling() const;

private:
    PlayerRoll(const PlayerRoll&) = delete;
    PlayerRoll& operator=(const PlayerRoll&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<Stamina> mStamina;
    //ローリング距離
    float mRollingDistance;
    //ローリング中か
    bool mIsRolling;
    //ローリングモーション時間
    std::unique_ptr<Time> mRollingMotionTime;
    //ローリング開始地点
    Vector3 mRollingStartPoint;
    //ローリング終了地点
    Vector3 mRollingEndPoint;
    //ローリングボタンを離すべきか
    bool mShouldReleaseRollingButton;
    //ローリングの消費スタミナ量
    float mRollingStaminaAmount;
};
