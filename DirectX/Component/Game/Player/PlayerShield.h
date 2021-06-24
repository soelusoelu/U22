#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;
class AABBCollider;

class PlayerShield
    : public Component
{
public:
    PlayerShield();
    ~PlayerShield();
    void setShield(const std::shared_ptr<GameObject>& shield);
    const GameObject& getShield() const;

private:
    PlayerShield(const PlayerShield&) = delete;
    PlayerShield& operator=(const PlayerShield&) = delete;

private:
    std::shared_ptr<GameObject> mShield;

    static constexpr unsigned LEFT_HAND_BONE_NO = 17;
};
