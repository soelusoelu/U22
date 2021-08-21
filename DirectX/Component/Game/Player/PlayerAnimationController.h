#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class PlayerMove;
class PlayerCrouch;
class BulletShooter;

class PlayerAnimationController
    : public Component
{
public:
    PlayerAnimationController();
    ~PlayerAnimationController();
    virtual void start() override;
    virtual void update() override;

private:
    PlayerAnimationController(const PlayerAnimationController&) = delete;
    PlayerAnimationController& operator=(const PlayerAnimationController&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<PlayerMove> mMove;
    std::shared_ptr<PlayerCrouch> mCrouch;
    std::shared_ptr<BulletShooter> mShooter;
};
