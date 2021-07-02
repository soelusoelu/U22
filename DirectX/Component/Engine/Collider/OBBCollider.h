#pragma once

#include "Collider.h"
#include "../../../Collision/Collision.h"
#include <memory>

class MeshComponent;

class OBBCollider
    : public Collider
{
public:
    OBBCollider();
    ~OBBCollider();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //OBBを取得する
    const OBB& getOBB() const;
    //影響されるボーンを設定する
    void setBone(unsigned boneNo);

private:
    OBBCollider(const OBBCollider&) = delete;
    OBBCollider& operator=(const OBBCollider&) = delete;

private:
    OBB mOBB;
    std::shared_ptr<MeshComponent> mMesh;
    int mBoneNo;
};
