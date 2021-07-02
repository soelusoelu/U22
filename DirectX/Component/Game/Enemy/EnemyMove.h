#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class Transform3D;
class SkinMeshComponent;

class EnemyMove
    : public Component
{
public:
    EnemyMove();
    ~EnemyMove();
    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    void originalUpdate(const Transform3D& player);

private:
    EnemyMove(const EnemyMove&) = delete;
    EnemyMove& operator=(const EnemyMove&) = delete;

    void walk(const Vector3& playerPosition);
    void rotate(const Vector3& playerPosition);

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    float mWalkSpeed;
    float mRotateSpeed;
    float mToPlayerLimitDistance;
};
