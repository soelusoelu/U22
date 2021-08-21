#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
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

private:
    //被ダメージ時
    void takeDamage();
    //無敵中か
    bool isInvincible() const;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<HitPoint> mHP;
    std::unique_ptr<Time> mInvincibleTime;
};
