#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class Transform3D;
class Time;

class EnemyAttack
    : public Component
{
public:
    EnemyAttack();
    ~EnemyAttack();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void attack(const Transform3D& player);
    bool isAttacking() const;
    int getDamage() const;

private:
    EnemyAttack(const EnemyAttack&) = delete;
    EnemyAttack& operator=(const EnemyAttack&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::unique_ptr<Time> mMotionTimer;
    int mDamage;
    float mAttackRangeDistance;
};
