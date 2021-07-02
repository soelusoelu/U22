#include "PlayerAttack.h"
#include "PlayerMotions.h"
#include "PlayerWeapon.h"
#include "Stamina.h"
#include "../../Engine/Collider/AABBCollider.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"
#include "../../../Utility/JsonHelper.h"

PlayerAttack::PlayerAttack()
    : Component()
    , mAnimation(nullptr)
    , mStamina(nullptr)
    , mWeapon(nullptr)
    , mMotionTimer(std::make_unique<Time>())
    , mAttackMotionTime{ 0.f, 0.f, 0.f, 0.f }
    , mCoolTimeUntilAdditionalAttack{ 0.f, 0.f }
    , mAttackStaminaAmount(0.f)
{
}

PlayerAttack::~PlayerAttack() = default;

void PlayerAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mStamina = getComponent<Stamina>();
    mWeapon = getComponent<PlayerWeapon>();

    //各モーションの時間を取得
    setMotionTime(FIRST_ATTACK_START_NO, PlayerMotions::FIRST_ATTACK_START);
    setMotionTime(SECOND_ATTACK_START_NO, PlayerMotions::SECOND_ATTACK_START);
    setMotionTime(FIRST_ATTACK_END_NO, PlayerMotions::FIRST_ATTACK_END);
    setMotionTime(SECOND_ATTACK_END_NO, PlayerMotions::SECOND_ATTACK_END);
}

void PlayerAttack::update() {
    //攻撃中以外無視
    if (!isAttackOperating()) {
        return;
    }

    if (Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        if (canSecondAttack()) {
            secondAttack();
        }
        if (canSecondToFirstAttack()) {
            firstAttack();
        }
    }

    mMotionTimer->update();
    if (mMotionTimer->isTime()) {
        mMotionTimer->reset();

        if (isAttacking()) {
            endAttack();

            //即下記の終了条件に当てはまるのを防ぐために
            return;
        }

        if (isAttackEnding()) {
            endAttackOperation();
        }
    }
}

void PlayerAttack::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSetFloat(
        mCoolTimeUntilAdditionalAttack[FIRST_ATTACK_START_NO],
        "firstLowestCoolTimeUpToAdditionalAttack",
        inObj,
        alloc,
        mode
    );
    JsonHelper::getSetFloat(
        mCoolTimeUntilAdditionalAttack[SECOND_ATTACK_START_NO],
        "secondLowestCoolTimeUpToAdditionalAttack",
        inObj,
        alloc,
        mode
    );
    JsonHelper::getSetFloat(mAttackStaminaAmount, "attackStaminaAmount", inObj, alloc, mode);
}

void PlayerAttack::originalUpdate() {
    if (!Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        return;
    }

    if (canFirstAttack()) {
        firstAttack();
    }
}

bool PlayerAttack::isAttackOperating() const {
    if (isAttacking()) {
        return true;
    }
    if (isAttackEnding()) {
        return true;
    }

    return false;
}

bool PlayerAttack::isAttacking() const {
    auto motionNo = mAnimation->getCurrentMotionNumber();
    if (motionNo == PlayerMotions::FIRST_ATTACK_START) {
        return true;
    }
    if (motionNo == PlayerMotions::SECOND_ATTACK_START) {
        return true;
    }

    return false;
}

bool PlayerAttack::isAttackEnding() const {
    auto motionNo = mAnimation->getCurrentMotionNumber();
    if (motionNo == PlayerMotions::FIRST_ATTACK_END) {
        return true;
    }
    if (motionNo == PlayerMotions::SECOND_ATTACK_END) {
        return true;
    }

    return false;
}

void PlayerAttack::setMotionTime(unsigned index, unsigned motionNo) {
    mAttackMotionTime[index] = static_cast<float>(mAnimation->getMotion(motionNo).numFrame) / 60.f;
}

void PlayerAttack::changeMotion(unsigned motionNo, bool isLoop) {
    mAnimation->changeMotion(motionNo);
    mAnimation->setLoop(isLoop);
}

void PlayerAttack::changeLimitTimer(float time) {
    mMotionTimer->setLimitTime(time);
    mMotionTimer->reset();
}

void PlayerAttack::firstAttack() {
    changeMotion(PlayerMotions::FIRST_ATTACK_START, false);
    changeLimitTimer(mAttackMotionTime[FIRST_ATTACK_START_NO]);
    //スタミナが回復しないように設定
    mStamina->setHealFlag(false);
    //武器の当たり判定を有効化
    mWeapon->getWeaponCollider().enabled();
}

void PlayerAttack::secondAttack() {
    changeMotion(PlayerMotions::SECOND_ATTACK_START, false);
    changeLimitTimer(mAttackMotionTime[SECOND_ATTACK_START_NO]);
}

void PlayerAttack::endAttack() {
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

void PlayerAttack::endAttackOperation() {
    changeMotion(PlayerMotions::IDOL, true);
    mStamina->setHealFlag(true);
}

void PlayerAttack::firstAttackEnd() {
    changeMotion(PlayerMotions::FIRST_ATTACK_END, false);
    changeLimitTimer(mAttackMotionTime[FIRST_ATTACK_END_NO]);
}

void PlayerAttack::secondAttackEnd() {
    changeMotion(PlayerMotions::SECOND_ATTACK_END, false);
    changeLimitTimer(mAttackMotionTime[SECOND_ATTACK_END_NO]);
}

bool PlayerAttack::canFirstAttack() const {
    if (isAttackOperating()) {
        return false;
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
    if (mMotionTimer->getCountUpTime() < mCoolTimeUntilAdditionalAttack[FIRST_ATTACK_START_NO]) {
        return false;
    }
    if (!mStamina->use(mAttackStaminaAmount)) {
        return false;
    }

    return true;
}

bool PlayerAttack::canSecondToFirstAttack() const {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::SECOND_ATTACK_START) {
        return false;
    }
    if (mMotionTimer->getCountUpTime() < mCoolTimeUntilAdditionalAttack[SECOND_ATTACK_START_NO]) {
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
