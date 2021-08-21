#include "PlayerWeapon.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Transform/ParentChildRelationship.h"
#include "../../../Transform/Transform3D.h"

PlayerWeapon::PlayerWeapon()
    : Component()
    , mWeapon(nullptr)
{
}

PlayerWeapon::~PlayerWeapon() = default;

void PlayerWeapon::setWeapon(const std::shared_ptr<GameObject>& weapon) {
    mWeapon = weapon;

    //装備を設定する
    const auto& rightHand = getComponent<SkinMeshComponent>()->getBoneCurrentFrameMatrix()[RIGHT_HAND_BONE_NO];
    transform().getParentChildRelation().setEquipment(weapon, &rightHand);
}

const GameObject& PlayerWeapon::getWeapon() const {
    return *mWeapon;
}
