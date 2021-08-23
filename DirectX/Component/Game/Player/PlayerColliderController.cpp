#include "PlayerColliderController.h"
#include "PlayerMotions.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../../Engine/Collider/Collider.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"

PlayerColliderController::PlayerColliderController()
    : Component()
    , mAnimation(nullptr)
    , mHP(nullptr)
    , mInvincibleTime(std::make_unique<Time>())
{
}

PlayerColliderController::~PlayerColliderController() = default;

void PlayerColliderController::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mHP = getComponent<HitPoint>();

    //被ダメージ時モーション時間を無敵時間に設定する
    const auto& motion = mAnimation->getMotion(PlayerMotions::TAKE_DAMAGE);
    auto motionTime = static_cast<float>(motion.numFrame) / 60.f;
    mInvincibleTime->setLimitTime(motionTime);
}

void PlayerColliderController::update() {
    //死亡しているなら終了
    if (mAnimation->getCurrentMotionNumber() == PlayerMotions::DEAD) {
        return;
    }

    //無敵中なら
    if (isInvincible()) {
        mInvincibleTime->update();
        if (mInvincibleTime->isTime()) {
            mInvincibleTime->reset();

            mAnimation->changeMotion(PlayerMotions::IDOL);
            mAnimation->setLoop(true);
        }
    }
}

void PlayerColliderController::onCollisionEnter(Collider& other) {
    //無敵中なら終了
    if (isInvincible()) {
        return;
    }
    //死亡しているなら終了
    if (mAnimation->getCurrentMotionNumber() == PlayerMotions::DEAD) {
        return;
    }

    if (other.gameObject().tag() == "Enemy") {
        takeDamage();
        return;
    }
}

void PlayerColliderController::onDead(const std::function<void()>& f) {
    mOnDead += f;
}

void PlayerColliderController::takeDamage() {
    mHP->takeDamage(10);

    //HPが残ってるなら被ダメ、0なら死亡モーション
    unsigned nextMotion = 0;
    if (mHP->getHp() > 0) {
        nextMotion = PlayerMotions::TAKE_DAMAGE;
    } else {
        nextMotion = PlayerMotions::DEAD;
        mOnDead();
    }
    mAnimation->changeMotion(nextMotion);
    mAnimation->setLoop(false);

    Debug::log("damege!");
}

bool PlayerColliderController::isInvincible() const {
    return (mAnimation->getCurrentMotionNumber() == PlayerMotions::TAKE_DAMAGE);
}
