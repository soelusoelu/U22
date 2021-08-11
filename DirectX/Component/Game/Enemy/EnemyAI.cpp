﻿#include "EnemyAI.h"
#include "EnemyMove.h"
#include "Octopus.h"
#include "OctopusFootManager.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

EnemyAI::EnemyAI()
    : Component()
    , mPlayer(nullptr)
    , mOctopus(nullptr)
    , mMove(nullptr)
    , mAttackRange(0.f)
{
}

EnemyAI::~EnemyAI() = default;

void EnemyAI::start() {
    mOctopus = getComponent<Octopus>();
    mMove = getComponent<EnemyMove>();

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
}

void EnemyAI::drawInspector() {
    ImGuiWrapper::dragFloat("Attack Range", mAttackRange);
}

void EnemyAI::setPlayer(const std::shared_ptr<GameObject>& player) {
    mPlayer = player;
    mOnSetPlayer(player);
}

void EnemyAI::onSetPlayer(const std::function<void(const std::shared_ptr<GameObject>&)>& f) {
    mOnSetPlayer += f;
}

void EnemyAI::updateFootAlive() {
    const auto& t = transform();
    auto dist = Vector3::distance(t.getPosition(), mPlayer->transform().getPosition());
    if (dist < getAttackRange()) {

    } else {
        mMove->move();
    }
    if (mMove->shouldRotate()) {
        mMove->rotate();
    }
}

float EnemyAI::getAttackRange() const {
    return (mAttackRange * transform().getScale().z);
}
