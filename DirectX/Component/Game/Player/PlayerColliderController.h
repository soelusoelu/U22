#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class SkinMeshComponent;
class SoundComponent;
class HitPoint;
class Time;

class PlayerColliderController
    : public Component
{
public:
    PlayerColliderController();
    ~PlayerColliderController();
    PlayerColliderController(const PlayerColliderController&) = delete;
    PlayerColliderController& operator=(const PlayerColliderController&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void onCollisionEnter(Collider& other) override;

    void onDead(const std::function<void()>& f);

private:
    //被ダメージ時
    void takeDamage();
    //無敵中か
    bool isInvincible() const;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<SoundComponent> mSound;
    std::shared_ptr<HitPoint> mHP;
    std::unique_ptr<Time> mInvincibleTime;
    Function<void()> mOnDead;
};
