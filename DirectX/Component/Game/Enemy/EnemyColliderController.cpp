#include "EnemyColliderController.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../Weapon/WeaponDamage.h"
#include "../../Engine/Collider/AABBAnimationCollider.h"

EnemyColliderController::EnemyColliderController()
    : Component()
    , mCollider(nullptr)
    , mHP(nullptr)
{
}

EnemyColliderController::~EnemyColliderController() = default;

void EnemyColliderController::start() {
    mCollider = getComponent<AABBAnimationCollider>();
    mHP = getComponent<HitPoint>();

    mCollider->concatenate(HEAD, NOSE);
    mCollider->concatenate(HEAD, LEFT_EYE);
    mCollider->concatenate(HEAD, RIGHT_EYE);
}

void EnemyColliderController::onCollisionEnter(Collider& other) {
    if (other.gameObject().tag() == "Weapon") {
        auto wd = other.getComponent<WeaponDamage>();
        mHP->takeDamage(wd->getDamage());
    }
}
