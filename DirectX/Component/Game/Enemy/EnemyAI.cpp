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
    , mAttackRange(0.f)
    , mAttackAngle(0.f)
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

#ifdef _DEBUG
    //デバッグ時のみ攻撃範囲を可視化
    const auto& t = transform();
    auto p1 = t.getPosition() + Vector3::up * 10.f;
    Debug::renderLine(p1, p1 + t.forward() * getAttackRange(), ColorPalette::red);
#endif // _DEBUG
}

void EnemyAI::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mAttackRange, "attackRange", inObj, alloc, mode);
    JsonHelper::getSet(mAttackAngle, "attackAngle", inObj, alloc, mode);
}

void EnemyAI::drawInspector() {
    ImGuiWrapper::dragFloat("Attack Range", mAttackRange);
    ImGuiWrapper::dragFloat("Attack Angle", mAttackAngle);
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

    const auto& t = transform();
    auto dist = Vector3::distance(t.getPosition(), mPlayer->transform().getPosition());
    if (dist < getAttackRange()) {
        if (canAttackAngle()) {
            mAttack->attack();
        } else {
            //mMove->rotate();
        }
    } else {
        //mMove->move();
        //mMove->rotate();
    }
}

float EnemyAI::getAttackRange() const {
    return (mAttackRange * transform().getScale().z);
}

bool EnemyAI::canAttackAngle() const {
    auto toPlayer = Vector3::normalize(mPlayer->transform().getPosition() - transform().getPosition());
    auto dot = Vector3::dot(toPlayer, Vector3::forward);
    auto angle = Math::acos(dot);
    return (mAttackAngle < angle * 2.f);
}
