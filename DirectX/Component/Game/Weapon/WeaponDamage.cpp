#include "WeaponDamage.h"
#include "../../../Utility/LevelLoader.h"

WeaponDamage::WeaponDamage()
    : Component()
    , mDamage(0)
{
}

WeaponDamage::~WeaponDamage() = default;

void WeaponDamage::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getInt(inObj, "damage", mDamage);
}

int WeaponDamage::getDamage() const {
    return mDamage;
}
