#include "EnemyAI.h"
#include "EnemyAttack.h"
#include "EnemyMove.h"
#include "../../../Device/Time.h"
#include "../../../Math/Math.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

EnemyAI::EnemyAI()
    : Component()
    , mMove(nullptr)
    , mAttack(nullptr)
    , mPlayer(nullptr)
{
}

EnemyAI::~EnemyAI() = default;

void EnemyAI::start() {
    mMove = getComponent<EnemyMove>();
    mAttack = getComponent<EnemyAttack>();
}

void EnemyAI::update() {
    if (!mAttack->isAttacking()) {
        mMove->originalUpdate(mPlayer->transform());
    }
    mAttack->attack(mPlayer->transform());
}

void EnemyAI::setPlayer(const std::shared_ptr<GameObject>& player) {
    mPlayer = player;
}
