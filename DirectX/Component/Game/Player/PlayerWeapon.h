#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;
class MeshComponent;
class SkinMeshComponent;
class AnimationCPU;
class AABBCollider;

class PlayerWeapon
    : public Component
{
public:
    PlayerWeapon();
    ~PlayerWeapon();
    virtual void start() override;
    virtual void lateUpdate() override;
    void setWeapon(const std::shared_ptr<GameObject>& weapon);
    const GameObject& getWeapon() const;
    AABBCollider& getWeaponCollider() const;

private:
    PlayerWeapon(const PlayerWeapon&) = delete;
    PlayerWeapon& operator=(const PlayerWeapon&) = delete;

private:
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<AnimationCPU> mAnimationCpu;

    std::shared_ptr<GameObject> mWeapon;
    std::shared_ptr<AABBCollider> mWeaponCollider;

    static constexpr unsigned RIGHT_HAND_BONE_NO = 10;
    static constexpr unsigned LEFT_HAND_BONE_NO = 14;
};
