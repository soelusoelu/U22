#include "PlayerWeapon.h"
#include "../../Engine/Collider/AABBCollider.h"
#include "../../Engine/Mesh/AnimationCPU.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Transform/ParentChildRelationship.h"
#include "../../../Transform/Transform3D.h"

PlayerWeapon::PlayerWeapon()
    : Component()
    , mMesh(nullptr)
    , mAnimation(nullptr)
    , mAnimationCpu(nullptr)
    , mWeapon(nullptr)
    , mWeaponCollider(nullptr)
{
}

PlayerWeapon::~PlayerWeapon() = default;

void PlayerWeapon::start() {
    mMesh = getComponent<MeshComponent>();
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimationCpu = getComponent<AnimationCPU>();
}

void PlayerWeapon::lateUpdate() {
    const auto& curBones = mAnimation->getBoneCurrentFrameMatrix();
    const auto& rightHand = curBones[RIGHT_HAND_BONE_NO];
    const auto& vertices = mAnimationCpu->getCurrentMotionVertexPositions(0);

    auto& wt = mWeapon->transform();
    wt.setPosition(vertices[419]);
    wt.setRotation(rightHand.getQuaternion());
}

void PlayerWeapon::setWeapon(const std::shared_ptr<GameObject>& weapon) {
    mWeapon = weapon;
    mWeaponCollider = weapon->componentManager().getComponent<AABBCollider>();
    mWeaponCollider->disabled();

    //プレイヤーの子に設定する
    transform().getParentChildRelation().addChild(weapon);
}

const GameObject& PlayerWeapon::getWeapon() const {
    return *mWeapon;
}

AABBCollider& PlayerWeapon::getWeaponCollider() const {
    return *mWeaponCollider;
}
