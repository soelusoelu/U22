#include "PlayerShield.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Transform/ParentChildRelationship.h"
#include "../../../Transform/Transform3D.h"

PlayerShield::PlayerShield()
    : Component()
    , mShield(nullptr)
{
}

PlayerShield::~PlayerShield() = default;

void PlayerShield::setShield(const std::shared_ptr<GameObject>& shield) {
    mShield = shield;

    //装備を設定する
    const auto& leftHand = getComponent<SkinMeshComponent>()->getBoneCurrentFrameMatrix()[LEFT_HAND_BONE_NO];
    transform().getParentChildRelation().setEquipment(shield, &leftHand);
}

const GameObject& PlayerShield::getShield() const {
    return *mShield;
}
