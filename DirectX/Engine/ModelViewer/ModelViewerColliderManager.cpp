#include "ModelViewerColliderManager.h"
#include "../Camera/SimpleCamera.h"
#include "../DebugManager/DebugUtility/LineRenderer/LineRenderer3D.h"
#include "../../Component/Engine/Collider/ColliderDrawer.h"
#include "../../Component/Engine/Collider/OBBCollider.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../Component/Engine/Mesh/SkinMeshComponent.h"
#include "../../Input/Input.h"

ModelViewerColliderManager::ModelViewerColliderManager()
    : mMesh(nullptr)
    , mSkinMesh(nullptr)
{
}

ModelViewerColliderManager::~ModelViewerColliderManager() = default;

void ModelViewerColliderManager::initialize(IModelViewerCallback* callback) {
    callback->callbackModeChange([&](ModelViewerMode mode) { onModeChange(mode); });
    callback->callbackModelChange([&](GameObject& newModel) { onChangeModel(newModel); });
}

void ModelViewerColliderManager::update(LineRenderer3D& line, const SimpleCamera& camera) {
    //drawTPoseBone(line);

    for (const auto& obb : mObbColliders) {
        ColliderDrawer::drawOBB(line, obb->getOBB());
        const auto& target = obb->getOBB();
        const auto& center = target.center;
        const auto& rot = target.rotation;
        //line.renderLine(center, center + Vector3::transform(Vector3::right, rot) * 0.25f, ColorPalette::red);
        //line.renderLine(center, center + Vector3::transform(Vector3::up, rot) * 0.25f, ColorPalette::green);
        //line.renderLine(center, center + Vector3::transform(Vector3::forward, rot) * 0.25f, ColorPalette::blue);
    }
}

void ModelViewerColliderManager::drawTPoseBone(LineRenderer3D& line) const {
    if (!isAnimation()) {
        return;
    }

    auto animation = mMesh->getAnimation();
    auto boneCount = animation->getBoneCount();

    const auto& curBones = mSkinMesh->getBoneCurrentFrameMatrix();
    for (unsigned i = 0; i < boneCount; ++i) {
        const auto& bone = animation->getBone(i);
        //親がいないなら次へ
        if (!bone.parent) {
            continue;
        }

        //自身のボーンと親のボーンを線で繋ぐ
        line.renderLine(
            Vector3::transform(bone.initMat.getTranslation(), curBones[bone.number]),
            Vector3::transform(bone.parent->initMat.getTranslation(), curBones[bone.parent->number]),
            COLORS[i % 7]
        );
    }
}

void ModelViewerColliderManager::createObbCollider() {
    mObbColliders = mMesh->getComponents<OBBCollider>();

    //元があるなら終了
    if (!mObbColliders.empty()) {
        return;
    }
    //アニメーションしないなら終了
    if (!isAnimation()) {
        return;
    }

    auto animation = mMesh->getAnimation();
    auto boneCount = animation->getBoneCount();
    for (unsigned i = 0; i < boneCount; ++i) {
        //胴
        //if (i != 3) {
        //    continue;
        //}
        //骨盤
        //if (i == 26 || i == 30) {
        //    continue;
        //}

        const auto& bone = animation->getBone(i);
        //ボーンの親がいないなら次へ
        if (!bone.parent) {
            continue;
        }

        auto obb = mMesh->addComponent<OBBCollider>("OBBCollider");
        obb->setBone(i);
        mObbColliders.emplace_back(obb);
    }
}

bool ModelViewerColliderManager::isAnimation() const {
    return (mSkinMesh) ? true : false;
}

void ModelViewerColliderManager::onChangeModel(const GameObject& newModel) {
    const auto& cm = newModel.componentManager();
    mMesh = cm.getComponent<MeshComponent>();
    mSkinMesh = cm.getComponent<SkinMeshComponent>();

    if (!mObbColliders.empty()) {
        mObbColliders.clear();
    }
}

void ModelViewerColliderManager::onModeChange(ModelViewerMode mode) { 
    FillMode fillMode = FillMode::SOLID;

    if (mode == ModelViewerMode::COLLIDER_OPERATE) {
        if (isAnimation()) {
            //強制的にTポーズに変更する
            mSkinMesh->tPose();
        }

        //OBB作成
        createObbCollider();

        //ワイヤーフレーム表示に変更する
        fillMode = FillMode::WIREFRAME;
    }

    //描画方法を変更する
    mMesh->getComponent<MeshRenderer>()->setFillMode(fillMode);
}

const Vector3 ModelViewerColliderManager::COLORS[] = {
    ColorPalette::red, ColorPalette::blue, ColorPalette::yellow, ColorPalette::green,
    ColorPalette::lightBlue, ColorPalette::lightGreen, ColorPalette::lightPink, ColorPalette::lightYellow
};
