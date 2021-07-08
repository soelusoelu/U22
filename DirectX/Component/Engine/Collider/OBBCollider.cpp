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

    for (unsigned i = 0; i < meshCount; ++i) {
        const auto& vertices = mesh->getMeshVertices(i);
        auto& affected = affectedVertices[i];

        //ボーンが影響する頂点を取得する
        const auto size = vertices.size();
        for (unsigned j = 0; j < size; ++j) {
            const auto& v = vertices[j];

            for (int idx = 0; idx < 4; ++idx) {
                if (v.index[idx] != mBoneNo) {
                    continue;
                }
                if (v.weight[idx] < 0.5f) {
                    continue;
                }

                affected.emplace_back(j);
            }
        }
    }

    create(affectedVertices);
}

void OBBCollider::create(const std::vector<std::vector<unsigned>>& vertices) {
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
    float maxX = 0.f;
    float maxY = 0.f;
    float maxZ = 0.f;
    float maxStartX = 0.f;
    float maxStartY = 0.f;
    float maxStartZ = 0.f;
    float maxEndX = 0.f;
    float maxEndY = 0.f;
    float maxEndZ = 0.f;

    auto mesh = mMesh->getMesh();
    auto meshCount = mesh->getMeshCount();
    for (unsigned i = 0; i < meshCount; ++i) {
        const auto& verticesPos = mesh->getMeshVerticesPosition(i);
        const auto& affected = vertices[i];
        for (const auto& x : affected) {
            const auto& pos = verticesPos[x];

            test(maxX, pos, center, Vector3::back, rot, ray);
            test(maxY, pos, center, Vector3::right, rot, ray);
            test(maxZ, pos, center, Vector3::down, rot, ray);

            test(maxStartX, pos, ray.start, Vector3::back, rot, ray);
            test(maxStartY, pos, ray.start, Vector3::right, rot, ray);
            test(maxStartZ, pos, ray.start, Vector3::down, rot, ray);

            test(maxEndX, pos, ray.end, Vector3::back, rot, ray);
            test(maxEndY, pos, ray.end, Vector3::right, rot, ray);
            test(maxEndZ, pos, ray.end, Vector3::down, rot, ray);
        }
    }

    maxX = min(maxX, maxStartX, maxEndX);
    maxY = min(maxY, maxStartY, maxEndY);
    maxZ = min(maxZ, maxStartZ, maxEndZ);

    constexpr int x = 2.f;
    maxX = Math::sqrt(maxX);
    maxX /= x;
    maxY = Math::sqrt(maxY);
    maxY /= x;
    maxZ = Math::sqrt(maxZ);
    maxZ /= x;

    mOBB.center = center;
    mOBB.rotation = rot;
    //mOBB.extents = Vector3(maxZ, maxY, maxX);
    //mOBB.extents = Vector3(maxX, maxY, maxZ);
    mOBB.extents = Vector3(maxZ, maxY, toParent.length() / 2.f);
    //mOBB.extents = Vector3(maxY, maxZ, toParent.length() / 2.f);
}

void OBBCollider::test(float& out, const Vector3& target, const Vector3& pos, const Vector3& axis, const Quaternion& rot, const Ray& ray) {
    constexpr float angle1 = 60.f;
    constexpr float angle2 = 180.f - angle1;

    auto toVertPos = Vector3::normalize(target - pos);
    float dot = Vector3::dot(toVertPos, Vector3::transform(axis, rot));
    float angle = Math::acos(dot);
    if (angle < angle1 || angle2 < angle) {
        float length = ray.minDistanceSquare(target);
        if (length > out) {
            out = length;
        }
    }
}

float OBBCollider::min(float value1, float value2, float value3) {
    if (Math::nearZero(value1)) {
        value1 = value2;
    }
    if (Math::nearZero(value2)) {
        value2 = value3;
    }
    if (Math::nearZero(value3)) {
        return Math::Min(value1, value2);
    } else {
        return Math::Min(value1, Math::Min(value2, value3));
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
