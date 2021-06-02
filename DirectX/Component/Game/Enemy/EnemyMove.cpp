#include "EnemyMove.h"
#include "EnemyMotions.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Utility/StringUtil.h"

EnemyMove::EnemyMove()
    : Component()
    , mAnimation(nullptr)
    , mWalkSpeed(0.f)
    , mRotateSpeed(0.f)
    , mToPlayerLimitDistance(0.f)
{
}

EnemyMove::~EnemyMove() = default;

void EnemyMove::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->changeMotion(EnemyMotions::WALK);
}

void EnemyMove::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "walkSpeed", &mWalkSpeed);
    JsonHelper::getFloat(inObj, "rotateSpeed", &mRotateSpeed);
    JsonHelper::getFloat(inObj, "toPlayerLimitDistance", &mToPlayerLimitDistance);
}

void EnemyMove::originalUpdate(const Transform3D& player) {
    auto pPos = player.getPosition();
    //rotate(pPos);
    transform().lookAt(player);
    walk(pPos);
}

void EnemyMove::walk(const Vector3& playerPosition) {
    auto& t = transform();
    auto dist = Vector3::distance(t.getPosition(), playerPosition);
    if (dist < mToPlayerLimitDistance) {
        return;
    }
    t.translate(t.forward() * mWalkSpeed * Time::deltaTime);
}

void EnemyMove::rotate(const Vector3& playerPosition) {
    //プレイヤーから見たエネミーを-1から1に換算する
    //auto& t = transform();
    //auto e2p = Vector3::normalize(playerPosition - t.getPosition());
    //auto dot = Vector3::dot(t.forward(), e2p);
    //if (Math::equal(dot, 1.f)) {
    //    return;
    //}

    ////角度に変換する
    //auto angle = Math::acos(dot);
    //Debug::log(StringUtil::floatToString(dot));
    //auto axis = (dot > 0.f) ? Vector3::up : Vector3::up;
    //t.rotate(axis, mRotateSpeed * Time::deltaTime);
}
