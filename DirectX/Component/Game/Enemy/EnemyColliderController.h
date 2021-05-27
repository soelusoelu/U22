#pragma once

#include "../../Component.h"
#include <memory>

class AABBAnimationCollider;
class HitPoint;

class EnemyColliderController
    : public Component
{
public:
    EnemyColliderController();
    ~EnemyColliderController();
    virtual void start() override;
    virtual void onCollisionEnter(Collider& other) override;

private:
    EnemyColliderController(const EnemyColliderController&) = delete;
    EnemyColliderController& operator=(const EnemyColliderController&) = delete;

private:
    std::shared_ptr<AABBAnimationCollider> mCollider;
    std::shared_ptr<HitPoint> mHP;

    static constexpr unsigned HEAD = 0;
    static constexpr unsigned NOSE = 2;
    static constexpr unsigned LEFT_EYE = 5;
    static constexpr unsigned RIGHT_EYE = 6;
};
