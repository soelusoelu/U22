#pragma once

#include "Collider.h"
#include "../../../Collision/Collision.h"

class OBBCollider
    : public Collider
{
public:
    OBBCollider();
    ~OBBCollider();
    virtual void start() override;
    virtual void lateUpdate() override;

private:
    OBBCollider(const OBBCollider&) = delete;
    OBBCollider& operator=(const OBBCollider&) = delete;

    void renderCollision() const;

private:
    OBB mOBB;
};
