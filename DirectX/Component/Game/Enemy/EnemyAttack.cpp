﻿#include "EnemyAttack.h"
#include "EnemyMotions.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

EnemyAttack::EnemyAttack()
    : Component()
    , mAnimation(nullptr)
    , mMotionTimer(std::make_unique<Time>())
    , mDamage(0)
    , mAttackRangeDistance(0.f)
{
}

EnemyAttack::~EnemyAttack() = default;

void EnemyAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    const auto& motion = mAnimation->getMotion(EnemyMotions::ATTACK);
    mMotionTimer->setLimitTime(static_cast<float>(motion.numFrame / 60.f));
}

void EnemyAttack::update() {
    if (!isAttacking()) {
        return;
    }

    mMotionTimer->update();
    if (mMotionTimer->isTime()) {
        mMotionTimer->reset();

        mAnimation->changeMotion(EnemyMotions::WALK);
        mAnimation->setLoop(true);
    }
}

void EnemyAttack::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mDamage, "damage", inObj, alloc, mode);
    JsonHelper::getSet(mAttackRangeDistance, "attackRangeDistance", inObj, alloc, mode);
}

void EnemyAttack::attack(const Transform3D& player) {
    if (isAttacking()) {
        return;
    }
    auto dist = Vector3::distance(transform().getPosition(), player.getPosition());
    if (dist > mAttackRangeDistance) {
        return;
    }

    mAnimation->changeMotion(EnemyMotions::ATTACK);
    mAnimation->setLoop(false);
}

bool EnemyAttack::isAttacking() const {
    return (mAnimation->getCurrentMotionNumber() == EnemyMotions::ATTACK);
}

int EnemyAttack::getDamage() const {
    return mDamage;
}
