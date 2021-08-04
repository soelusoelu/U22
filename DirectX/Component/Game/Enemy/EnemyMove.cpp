#include "EnemyMove.h"
#include "EnemyAI.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Math/Math.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/Easing.h"
#include "../../../Utility/JsonHelper.h"

EnemyMove::EnemyMove()
    : Component()
    , mPlayer(nullptr)
    , mMoveSpeed(0.f)
    , mRotateSpeed(0.f)
{
}

EnemyMove::~EnemyMove() = default;

void EnemyMove::start() {
    getComponent<EnemyAI>()->onSetPlayer([&](const std::shared_ptr<GameObject>& player) { mPlayer = player; });
}

void EnemyMove::update() {
    auto& t = transform();
    auto toPlayer = Vector3::normalize(mPlayer->transform().getPosition() - t.getPosition());
    auto lookRot = Quaternion::lookRotation(toPlayer);

    auto f = mRotateSpeed * Time::deltaTime;
    f = Easing::easeInCubic(f);

    t.setRotation(Quaternion::slerp(t.getRotation(), lookRot, f));
}

void EnemyMove::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mMoveSpeed, "moveSpeed", inObj, alloc, mode);
    JsonHelper::getSet(mRotateSpeed, "rotateSpeed", inObj, alloc, mode);
}

void EnemyMove::drawInspector() {
    ImGuiWrapper::dragFloat("Move Speed", mMoveSpeed, 0.1f);
    ImGuiWrapper::dragFloat("Rotate Speed", mRotateSpeed, 0.1f);
}

void EnemyMove::originalUpdate() {
    auto& t = transform();
    auto toPlayer = Vector3::normalize(mPlayer->transform().getPosition() - t.getPosition());
    t.translate(toPlayer * mMoveSpeed * Time::deltaTime);
}
