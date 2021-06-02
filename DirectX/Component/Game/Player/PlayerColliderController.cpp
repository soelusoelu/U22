#include "PlayerColliderController.h"
#include "../Enemy/EnemyAttack.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../../Engine/Collider/AABBAnimationCollider.h"

PlayerColliderController::PlayerColliderController()
    : Component()
    , mCollider(nullptr)
    , mHP(nullptr)
{
}

PlayerColliderController::~PlayerColliderController() = default;

void PlayerColliderController::start() {
    mHP = getComponent<HitPoint>();
}

void PlayerColliderController::onCollisionEnter(Collider& other) {
    if (other.gameObject().tag() == "Enemy") {
        auto ea = other.getComponent<EnemyAttack>();
        if (ea->isAttacking()) {
            mHP->takeDamage(ea->getDamage());
        }
    }
}
