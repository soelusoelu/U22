#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;

class PlayerWeapon
    : public Component
{
public:
    PlayerWeapon();
    ~PlayerWeapon();
    void setWeapon(const std::shared_ptr<GameObject>& weapon);
    const GameObject& getWeapon() const;

private:
    PlayerWeapon(const PlayerWeapon&) = delete;
    PlayerWeapon& operator=(const PlayerWeapon&) = delete;

private:
    std::shared_ptr<GameObject> mWeapon;

    static constexpr unsigned RIGHT_HAND_BONE_NO = 9;
};
