#include "OBBCollider.h"
#include "ColliderDrawer.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/SkinMeshComponent.h"
#include "../../../Engine/DebugManager/DebugUtility/DebugUtility.h"
#include "../../../Utility/LevelLoader.h"

OBBCollider::OBBCollider()
    : Collider()
    , mOBB()
    , mMesh(nullptr)
    , mBoneNo(-1)
{
}

OBBCollider::~OBBCollider() = default;

void OBBCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    //mOBB = OBBCreater::create(mesh->getMesh()->getMeshVerticesPosition(0));
}

void OBBCollider::lateUpdate() {
    Collider::lateUpdate();

    if (mBoneNo != -1) {
        const auto& bone = mMesh->getAnimation()->getBone(mBoneNo);
        const auto parent = bone.parent;

        //auto animation = getComponent<SkinMeshComponent>();
        //const auto& curFrameBones = animation->getBoneCurrentFrameMatrix();

        auto bonePos = bone.initMat.getTranslation();
        auto parentPos = parent->initMat.getTranslation();
        auto toParent = (parentPos - bonePos);
        mOBB.center = (bonePos + parentPos) / 2.f;
        mOBB.rotation = Quaternion::lookRotation(Vector3::normalize(toParent));
        mOBB.extents = Vector3::one * 0.25f;
    }

    //当たり判定表示
    //ColliderDrawer::drawOBB(DebugUtility::instance().lineRenderer3D(), mOBB);
}

void OBBCollider::loadProperties(const rapidjson::Value& inObj) {
    //if (JsonHelper::getVector3Array(inObj, "vertices", &mVertices)) {
    //    mOBB = OBBCreater::create(mVertices);
    //}
}

void OBBCollider::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    //JsonHelper::setVector3Array(alloc, inObj, "vertices", mVertices);
}

const OBB& OBBCollider::getOBB() const {
    return mOBB;
}

void OBBCollider::setBone(unsigned boneNo) {
    mBoneNo = static_cast<int>(boneNo);
}
