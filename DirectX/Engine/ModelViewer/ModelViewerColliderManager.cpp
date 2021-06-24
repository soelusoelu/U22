#include "ModelViewerColliderManager.h"
#include "../DebugManager/DebugUtility/LineRenderer/LineRenderer3D.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../Component/Engine/Mesh/SkinMeshComponent.h"

ModelViewerColliderManager::ModelViewerColliderManager()
    : mSkinMesh(nullptr)
    , mAnimation(nullptr)
{
}

ModelViewerColliderManager::~ModelViewerColliderManager() = default;

void ModelViewerColliderManager::initialize(IModelViewerCallback* callback) {
    callback->callbackModeChange([&](ModelViewerMode mode) { onModeChange(mode); });
    callback->callbackModelChange([&](GameObject& newModel) { onChangeModel(newModel); });
}

void ModelViewerColliderManager::update(LineRenderer3D& line) {
    drawTPoseBone(line);
}

void ModelViewerColliderManager::drawTPoseBone(LineRenderer3D& line) const {
    auto boneCount = mAnimation->getBoneCount();

    for (unsigned i = 0; i < boneCount; ++i) {
        const auto& bone = mAnimation->getBone(i);
        //親がいないなら次へ
        if (!bone.parent) {
            continue;
        }

        //自身のボーンと親のボーンを線で繋ぐ
        line.renderLine(
            bone.initMat.getTranslation(),
            bone.parent->initMat.getTranslation(),
            COLORS[i % 7]
        );
    }
}

void ModelViewerColliderManager::onChangeModel(const GameObject& newModel) {
    const auto& cm = newModel.componentManager();
    mSkinMesh = cm.getComponent<SkinMeshComponent>();
    mAnimation = cm.getComponent<MeshComponent>()->getAnimation();
}

void ModelViewerColliderManager::onModeChange(ModelViewerMode mode) {
    FillMode fillMode = FillMode::SOLID;

    if (mode == ModelViewerMode::COLLIDER_OPERATE) {
        //強制的にTポーズに変更する
        mSkinMesh->tPose();
        //ワイヤーフレーム表示に変更する
        fillMode = FillMode::WIREFRAME;
    }

    //描画方法を変更する
    mSkinMesh->getComponent<MeshRenderer>()->setFillMode(fillMode);
}

const Vector3 ModelViewerColliderManager::COLORS[] = {
    ColorPalette::red, ColorPalette::blue, ColorPalette::yellow, ColorPalette::green,
    ColorPalette::lightBlue, ColorPalette::lightGreen, ColorPalette::lightPink, ColorPalette::lightYellow
};
