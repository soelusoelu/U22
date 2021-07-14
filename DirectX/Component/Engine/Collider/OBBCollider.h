#pragma once

#include "Collider.h"
#include "../../../Collision/Collision.h"
#include "../../../Mesh/Bone.h"
#include <memory>
#include <vector>

class MeshComponent;
class SkinMeshComponent;

class OBBCollider
    : public Collider
{
public:
    OBBCollider();
    ~OBBCollider();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    //OBBを取得する
    const OBB& getOBB() const;
    //影響されるボーンとその長さ[0, 1]を設定する
    void setBone(unsigned boneNo, float start = 0.f, float end = 1.f);

private:
    OBBCollider(const OBBCollider&) = delete;
    OBBCollider& operator=(const OBBCollider&) = delete;

    void create(
        const std::vector<std::vector<unsigned>>& vertices,
        const std::vector<std::vector<Vector3>>& vertices2
    );

    void test(
        float& outMin,
        float& outMax,
        const Vector3& target,
        const Vector3& pos,
        const Vector3& axis,
        const Quaternion& rot,
        const Ray& ray
    );

    void beforeComputeWorldMatrix();

    const Bone& getBone() const;
    Vector3 getBonePosition(const Bone& bone) const;

private:
    OBB mOBB;
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    //影響を受けるボーン
    int mBoneNo;
    //ボーンの開始地点
    float mBoneStart;
    //ボーンの終了地点
    float mBoneEnd;
};
