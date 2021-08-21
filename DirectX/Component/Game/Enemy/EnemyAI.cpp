#include "EnemyAI.h"
#include "EnemyMove.h"
#include "Octopus.h"
#include "OctopusFootAttack.h"
#include "OctopusFootManager.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

EnemyAI::EnemyAI()
    : Component()
    , mPlayer(nullptr)
    , mOctopus(nullptr)
    , mMove(nullptr)
    , mAttack(nullptr)
    , mAttackRangeDistance(0.f)
    , mAttackRangeAngle(0.f)
    , mIsDebugMode(false)
{
}

EnemyAI::~EnemyAI() = default;

void EnemyAI::start() {
    mOctopus = getComponent<Octopus>();
    mMove = getComponent<EnemyMove>();
    mAttack = getComponent<OctopusFootAttack>();

    //初期モーションはTPose(歩行)に
    getComponent<SkinMeshComponent>()->tPose();
}

void EnemyAI::update() {
    if (mOctopus->getFootManager().isFootAlive()) {
        updateFootAlive();
    }
}

void EnemyAI::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mAttackRangeDistance, "attackRange", inObj, alloc, mode);
    JsonHelper::getSet(mAttackRangeAngle, "attackAngle", inObj, alloc, mode);
}

void EnemyAI::drawInspector() {
    ImGuiWrapper::dragFloat("Attack Range", mAttackRangeDistance);
    ImGuiWrapper::dragFloat("Attack Angle", mAttackRangeAngle);
    ImGui::Checkbox("Is Debug Mode", &mIsDebugMode);
}

void EnemyAI::setPlayer(const std::shared_ptr<GameObject>& player) {
    mPlayer = player;
    mOnSetPlayer(player);
}

void EnemyAI::onSetPlayer(const std::function<void(const std::shared_ptr<GameObject>&)>& f) {
    mOnSetPlayer += f;
}

void EnemyAI::updateFootAlive() {
    //デバッグモード中は更新しない
    if (mIsDebugMode) {
        return;
    }

    //回転は常に行う
    mMove->rotate();

    //攻撃中なら終わるまで無視
    if (mAttack->isAttacking()) {
        return;
    }

    //状況に応じて行動
    if (canAttackRangeDistance()) {
        if (canAttackRangeAngle()) {
            mAttack->attack();
        }
    } else {
        mMove->move();
    }
}

float EnemyAI::getAttackRangeDistance() const {
    return (mAttackRangeDistance * transform().getScale().z);
}

bool EnemyAI::canAttackRangeDistance() const {
    auto dist = Vector3::distance(transform().getPosition(), mPlayer->transform().getPosition());
    return (dist < getAttackRangeDistance());
}

float EnemyAI::getToPlayerAngle() const {
    const auto& t = transform();
    auto toPlayer = Vector3::normalize(mPlayer->transform().getPosition() - t.getPosition());
    auto dot = Vector3::dot(toPlayer, t.forward());
    return Math::acos(dot);
}

bool EnemyAI::canAttackRangeAngle() const {
    return (mAttackRangeAngle > getToPlayerAngle() * 2.f);
}
