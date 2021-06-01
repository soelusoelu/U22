#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;

class EnemyAnimationController
    : public Component
{
public:
    EnemyAnimationController();
    ~EnemyAnimationController();
    virtual void start() override;

private:
    EnemyAnimationController(const EnemyAnimationController&) = delete;
    EnemyAnimationController& operator=(const EnemyAnimationController&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
};
