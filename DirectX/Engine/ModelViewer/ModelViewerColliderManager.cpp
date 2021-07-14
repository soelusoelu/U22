#include "ModelViewerColliderManager.h"
#include "../Camera/SimpleCamera.h"
#include "../DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../DebugManager/DebugUtility/LineRenderer/LineRenderer3D.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Collider/ColliderDrawer.h"
#include "../../Component/Engine/Collider/OBBCollider.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../Component/Engine/Mesh/SkinMeshComponent.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"

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

    const auto& mouse = Input::mouse();
    const auto& mousePos = mouse.getMousePosition();
    if (mousePos.x < Window::width() && mousePos.y < Window::height()) {
        auto ray = camera.screenToRay(mousePos);
        for (const auto& obb : mObbColliders) {
            Vector3 color = ColorPalette::lightGreen;
            if (Intersect::intersectRayOBB(ray, obb->getOBB())) {
                color = ColorPalette::red;
            }

            ColliderDrawer::drawOBB(line, obb->getOBB(), color);
        }
    }

    //for (const auto& obb : mObbColliders) {
    //    ColliderDrawer::drawOBB(line, obb->getOBB());
    //}
}

void ModelViewerColliderManager::drawGUI() {
    ImGui::Text("OBB");

    mObbColliders[0]->drawInspector();
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

        //OBB分割数
        constexpr int DIV_COUNT = 1;
        for (int j = 0; j < DIV_COUNT; ++j) {
            auto obb = mMesh->addComponent<OBBCollider>("OBBCollider");
            float start = static_cast<float>(j) / static_cast<float>(DIV_COUNT);
            float end = static_cast<float>(j + 1) / static_cast<float>(DIV_COUNT);
            obb->setBone(i, start, end);
            mObbColliders.emplace_back(obb);
        }
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
