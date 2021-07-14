#include "OBBCollider.h"
#include "ColliderDrawer.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/SkinMeshComponent.h"
#include "../../../Engine/DebugManager/DebugUtility/DebugUtility.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

OBBCollider::OBBCollider()
    : Collider()
    , mOBB()
    , mMesh(nullptr)
    , mAnimation(nullptr)
    , mBoneNo(-1)
{
}

OBBCollider::~OBBCollider() = default;

void OBBCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    mAnimation = getComponent<SkinMeshComponent>();

    transform().callbackBeforeComputeWorldMatrix([&] { beforeComputeWorldMatrix(); });
}

void OBBCollider::lateUpdate() {
    Collider::lateUpdate();

    //当たり判定表示
    //ColliderDrawer::drawOBB(DebugUtility::instance().lineRenderer3D(), mOBB);
}

void OBBCollider::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
}

const OBB& OBBCollider::getOBB() const {
    return mOBB;
}

void OBBCollider::setBone(unsigned boneNo) {
    mBoneNo = static_cast<int>(boneNo);

    const auto& bone = mMesh->getAnimation()->getBone(mBoneNo);
    const auto parent = bone.parent;
    auto bonePos = bone.initMat.getTranslation();
    auto parentPos = parent->initMat.getTranslation();

    const auto mesh = mMesh->getMesh();
    auto meshCount = mesh->getMeshCount();
    //ボックスを構成する頂点番号群
    std::vector<std::vector<unsigned>> affectedVertices(meshCount);
    std::vector<std::vector<Vector3>> affectedVertices2(meshCount);

    for (unsigned i = 0; i < meshCount; ++i) {
        const auto& vertices = mesh->getMeshVertices(i);
        auto& affected = affectedVertices[i];
        auto& affected2 = affectedVertices2[i];

        //ボーンが影響する頂点を取得する
        const auto size = vertices.size();
        for (unsigned j = 0; j < size; ++j) {
            const auto& v = vertices[j];

            for (int idx = 0; idx < 4; ++idx) {
                if (v.index[idx] != mBoneNo) {
                    continue;
                }
                //if (v.weight[idx] < 0.05f) {
                //    continue;
                //}

                affected.emplace_back(j);
                affected2.emplace_back(v.pos);
            }
        }
    }

    create(affectedVertices, affectedVertices2);
}

void OBBCollider::create(
    const std::vector<std::vector<unsigned>>& vertices,
    const std::vector<std::vector<Vector3>>& vertices2
) {
    const auto& bone = mMesh->getAnimation()->getBone(mBoneNo);
    const auto parent = bone.parent;
    const auto& curBones = mAnimation->getBoneCurrentFrameMatrix();
    auto bonePos = Vector3::transform(bone.initMat.getTranslation(), curBones[mBoneNo]);
    auto parentPos = Vector3::transform(parent->initMat.getTranslation(), curBones[parent->number]);
    auto toParent = (parentPos - bonePos);
    auto center = (bonePos + parentPos) / 2.f;

    Ray ray;
    ray.start = bonePos;
    ray.end = parentPos;

    auto rot = Quaternion::lookRotation(Vector3::normalize(toParent));
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = FLT_MIN;
    float maxY = FLT_MIN;
    float maxZ = FLT_MIN;

    auto mesh = mMesh->getMesh();
    auto meshCount = mesh->getMeshCount();
    for (unsigned i = 0; i < meshCount; ++i) {
        const auto& verticesPos = mesh->getMeshVerticesPosition(i);
        const auto& affected = vertices[i];
        for (const auto& x : affected) {
            const auto& pos = verticesPos[x];

            test(minX, maxX, pos, center, Vector3::right, rot, ray);
            test(minY, maxY, pos, center, Vector3::up, rot, ray);
            test(minZ, maxZ, pos, center, Vector3::forward, rot, ray);

            test(minX, maxX, pos, ray.start, Vector3::right, rot, ray);
            test(minY, maxY, pos, ray.start, Vector3::up, rot, ray);
            test(minZ, maxZ, pos, ray.start, Vector3::forward, rot, ray);

            test(minX, maxX, pos, ray.end, Vector3::right, rot, ray);
            test(minY, maxY, pos, ray.end, Vector3::up, rot, ray);
            test(minZ, maxZ, pos, ray.end, Vector3::forward, rot, ray);
        }
    }

    //最低と最高の平均
    //maxX = (minX + maxX) / 2.f;
    //maxY = (minY + maxY) / 2.f;
    //maxZ = (minZ + maxZ) / 2.f;
    //最低値に置き換える
    maxX = minX;
    maxY = minY;
    maxZ = minZ;

    //2乗されている距離を正しい距離に直す
    maxX = Math::sqrt(maxX);
    maxY = Math::sqrt(maxY);
    maxZ = Math::sqrt(maxZ);

    mOBB.center = center;
    mOBB.rotation = rot;
    //mOBB.extents = Vector3(maxX, maxY, maxZ);
    mOBB.extents = Vector3(maxX, maxY, toParent.length() / 2.f);
}

void OBBCollider::test(
    float& outMin,
    float& outMax,
    const Vector3& target,
    const Vector3& pos,
    const Vector3& axis,
    const Quaternion& rot,
    const Ray& ray
) {
    constexpr float ANGLE1 = 45.f;
    constexpr float ANGLE2 = 180.f - ANGLE1;

    auto toVertPos = Vector3::normalize(target - pos);
    float dot = Vector3::dot(toVertPos, Vector3::transform(axis, rot));
    float angle = Math::acos(dot);
    if (angle < ANGLE1 || ANGLE2 < angle) {
        float distSq = ray.minDistanceSquare(target);
        if (distSq < outMin) {
            outMin = distSq;
        }
        if (distSq > outMax) {
            outMax = distSq;
        }
    }
}

void OBBCollider::beforeComputeWorldMatrix() {
    const auto& bone = mMesh->getAnimation()->getBone(mBoneNo);
    const auto parent = bone.parent;
    const auto& curBones = mAnimation->getBoneCurrentFrameMatrix();
    auto bonePos = Vector3::transform(bone.initMat.getTranslation(), curBones[mBoneNo]);
    auto parentPos = Vector3::transform(parent->initMat.getTranslation(), curBones[parent->number]);
    auto toParent = (parentPos - bonePos);

    mOBB.center = (bonePos + parentPos) / 2.f;
    mOBB.rotation = Quaternion::lookRotation(Vector3::normalize(toParent));
}
