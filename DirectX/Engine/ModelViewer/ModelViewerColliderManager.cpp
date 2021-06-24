#include "ModelViewerColliderManager.h"
#include "VertexSelector.h"
#include "../Camera/SimpleCamera.h"
#include "../DebugManager/DebugUtility/LineRenderer/LineRenderer3D.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../Component/Engine/Mesh/SkinMeshComponent.h"
#include "../../Input/Input.h"

ModelViewerColliderManager::ModelViewerColliderManager()
    : mMesh(nullptr)
    , mSkinMesh(nullptr)
    , mAnimation(nullptr)
{
}

ModelViewerColliderManager::~ModelViewerColliderManager() = default;

void ModelViewerColliderManager::initialize(IModelViewerCallback* callback) {
    callback->callbackModeChange([&](ModelViewerMode mode) { onModeChange(mode); });
    callback->callbackModelChange([&](GameObject& newModel) { onChangeModel(newModel); });
}

void ModelViewerColliderManager::update(LineRenderer3D& line, const SimpleCamera& camera) {
    drawTPoseBone(line);

    Vector3 selected;
    if (VertexSelector::selectVertexFromModel(selected, mMesh->getMesh(), camera, 0.05f)) {
        if (Input::mouse().getMouseButton(MouseCode::LeftButton)) {
            line.renderLine(selected, selected + Vector3::right * 3.f, ColorPalette::red);
            line.renderLine(selected, selected + Vector3::up * 3.f, ColorPalette::red);
            line.renderLine(selected, selected + Vector3::forward * 3.f, ColorPalette::red);
        }
    }
}

void ModelViewerColliderManager::drawTPoseBone(LineRenderer3D& line) const {
    if (!isAnimation()) {
        return;
    }

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

bool ModelViewerColliderManager::isAnimation() const {
    return (mSkinMesh) ? true : false;
}

void ModelViewerColliderManager::onChangeModel(const GameObject& newModel) {
    const auto& cm = newModel.componentManager();
    mMesh = cm.getComponent<MeshComponent>();
    mSkinMesh = cm.getComponent<SkinMeshComponent>();
    mAnimation = mMesh->getAnimation();
}

void ModelViewerColliderManager::onModeChange(ModelViewerMode mode) {
    FillMode fillMode = FillMode::SOLID;

    if (mode == ModelViewerMode::COLLIDER_OPERATE) {
        if (isAnimation()) {
            //強制的にTポーズに変更する
            mSkinMesh->tPose();
        }

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
