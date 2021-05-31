#include "PlayerAttack.h"
#include "PlayerMotions.h"
#include "PlayerWeapon.h"
#include "Stamina.h"
#include "../../Engine/Collider/AABBCollider.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"
#include "../../../Utility/LevelLoader.h"

PlayerAttack::PlayerAttack()
    : Component()
    , mAnimation(nullptr)
    , mStamina(nullptr)
    , mWeapon(nullptr)
    , mAttackMotionElapsedTimer(std::make_unique<Time>())
    , mAttackMotionTime{ 0.f, 0.f, 0.f, 0.f }
    , mLowestCoolTimeUpToAdditionalAttack{ 0.f, 0.f }
    , mAttackStaminaAmount(0.f)
{
}

PlayerAttack::~PlayerAttack() = default;

void PlayerAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mStamina = getComponent<Stamina>();
    mWeapon = getComponent<PlayerWeapon>();

    const auto& firstAttack = mAnimation->getMotion(PlayerMotions::FIRST_ATTACK_START);
    mAttackMotionTime[FIRST_ATTACK_START_NO] = static_cast<float>(firstAttack.numFrame) / 60.f;
    const auto& secondAttack = mAnimation->getMotion(PlayerMotions::SECOND_ATTACK_START);
    mAttackMotionTime[SECOND_ATTACK_START_NO] = static_cast<float>(secondAttack.numFrame) / 60.f;
    const auto& firstAttackEnd = mAnimation->getMotion(PlayerMotions::FIRST_ATTACK_END);
    mAttackMotionTime[FIRST_ATTACK_END_NO] = static_cast<float>(firstAttackEnd.numFrame) / 60.f;
    const auto& secondAttackEnd = mAnimation->getMotion(PlayerMotions::SECOND_ATTACK_END);
    mAttackMotionTime[SECOND_ATTACK_END_NO] = static_cast<float>(secondAttackEnd.numFrame) / 60.f;
}

void PlayerAttack::update() {
    auto motionNo = mAnimation->getCurrentMotionNumber();
    if (motionNo == PlayerMotions::FIRST_ATTACK_START
        || motionNo == PlayerMotions::SECOND_ATTACK_START) {
        attackEnd();
    }

    if (motionNo == PlayerMotions::FIRST_ATTACK_END
        || motionNo == PlayerMotions::SECOND_ATTACK_END) {
        updateEndAttack();
    }
}

void PlayerAttack::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(
        inObj,
        "firstLowestCoolTimeUpToAdditionalAttack",
        &mLowestCoolTimeUpToAdditionalAttack[FIRST_ATTACK_START_NO]
    );
    JsonHelper::getFloat(
        inObj,
        "secondLowestCoolTimeUpToAdditionalAttack",
        &mLowestCoolTimeUpToAdditionalAttack[SECOND_ATTACK_START_NO]
    );
    JsonHelper::getFloat(inObj, "attackStaminaAmount", &mAttackStaminaAmount);
}

void PlayerAttack::originalUpdate() {
    if (!Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        return;
    }

    if (canFirstAttack()) {
        firstAttack();
    }
    if (canSecondAttack()) {
        secondAttack();
    }
}

bool PlayerAttack::isAttacking() const {
    auto motionNo = mAnimation->getCurrentMotionNumber();
    if (motionNo == PlayerMotions::FIRST_ATTACK_START) {
        return true;
    }
    if (motionNo == PlayerMotions::FIRST_ATTACK_END) {
        return true;
    }
    if (motionNo == PlayerMotions::SECOND_ATTACK_START) {
        return true;
    }
    if (motionNo == PlayerMotions::SECOND_ATTACK_END) {
        return true;
    }

    return false;
}

void PlayerAttack::updateEndAttack() {
    if (!updateTimer()) {
        return;
    }

    mAnimation->changeMotion(PlayerMotions::IDOL);
    mAnimation->setLoop(true);
    mStamina->setHealFlag(true);
}

void PlayerAttack::firstAttack() {
    mAnimation->changeMotion(PlayerMotions::FIRST_ATTACK_START);
    mAnimation->setLoop(false);
    mStamina->setHealFlag(false);
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[FIRST_ATTACK_START_NO]);
    mAttackMotionElapsedTimer->reset();
    mWeapon->getWeaponCollider().enabled();
}

void PlayerAttack::secondAttack() {
    mAnimation->changeMotion(PlayerMotions::SECOND_ATTACK_START);
    mAnimation->setLoop(false);
    mStamina->setHealFlag(false);
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[SECOND_ATTACK_START_NO]);
    mAttackMotionElapsedTimer->reset();
}

void PlayerAttack::attackEnd() {
    if (!updateTimer()) {
        return;
    }

    //1回目の攻撃終了か
    if (isEndFirstAttack()) {
        firstAttackEnd();
    }
    //2回目の攻撃終了か
    if (isEndSecondAttack()) {
        secondAttackEnd();
    }

    //攻撃終了時の共通処理
    mWeapon->getWeaponCollider().disabled();
}

void PlayerAttack::firstAttackEnd() {
    mAnimation->changeMotion(PlayerMotions::FIRST_ATTACK_END);
    mAnimation->setLoop(false);
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[FIRST_ATTACK_END_NO]);
    mAttackMotionElapsedTimer->reset();
}

void PlayerAttack::secondAttackEnd() {
    mAnimation->changeMotion(PlayerMotions::SECOND_ATTACK_END);
    mAnimation->setLoop(false);
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[SECOND_ATTACK_END_NO]);
    mAttackMotionElapsedTimer->reset();
}

bool PlayerAttack::updateTimer() {
    mAttackMotionElapsedTimer->update();

    if (mAttackMotionElapsedTimer->isTime()) {
        mAttackMotionElapsedTimer->reset();
        return true;
    }

    return false;
}

bool PlayerAttack::canFirstAttack() const {
    if (isAttacking()) {
        if (mAnimation->getCurrentMotionNumber() != PlayerMotions::SECOND_ATTACK_START) {
            return false;
        }
        if (mAttackMotionElapsedTimer->getCountUpTime() < mLowestCoolTimeUpToAdditionalAttack[SECOND_ATTACK_START_NO]) {
            return false;
        }
        if (mAttackMotionElapsedTimer->isTime()) {
            mAttackMotionElapsedTimer->reset();
            return false;
        }
    }
    if (!mStamina->use(mAttackStaminaAmount)) {
        return false;
    }

    return true;
}

bool PlayerAttack::canSecondAttack() const {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::FIRST_ATTACK_START) {
        return false;
    }
    if (mAttackMotionElapsedTimer->getCountUpTime() < mLowestCoolTimeUpToAdditionalAttack[FIRST_ATTACK_START_NO]) {
        return false;
    }
    if (mAttackMotionElapsedTimer->isTime()) {
        mAttackMotionElapsedTimer->reset();
        return false;
    }
    if (!mStamina->use(mAttackStaminaAmount)) {
        return false;
    }

    return true;
}

bool PlayerAttack::isEndFirstAttack() const {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::FIRST_ATTACK_START) {
        return false;
    }

    return true;
}

bool PlayerAttack::isEndSecondAttack() const {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::SECOND_ATTACK_START) {
        return false;
    }

    return true;
}
