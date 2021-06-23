#include "ModelViewer.h"
#include "AnimationViewer.h"
#include "ModelViewCamera.h"
#include "ModelViewerLight.h"
#include "ModelViewerPlane.h"
#include "SaveModelViewerCollider.h"
#include "../AssetsRenderer/AssetsRenderTexture.h"
#include "../Camera/SimpleCamera.h"
#include "../DebugManager/DebugManager.h"
#include "../DebugManager/DebugLayer/DebugLayer.h"
#include "../DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../Mesh/MeshManager.h"
#include "../../System/Window.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Transform/Transform3D.h"

ModelViewer::ModelViewer()
    : mEngineModeGetter(nullptr)
    , mAssetsTextureGetter(nullptr)
    , mMeshManager(std::make_unique<MeshManager>())
    , mPlane(std::make_unique<ModelViewerPlane>())
    , mModelViewCamera(std::make_unique<ModelViewCamera>())
    , mAnimationViewer(std::make_unique<AnimationViewer>())
    , mLight(std::make_unique<ModelViewerLight>())
    , mGuiSizeX(0.f)
{
}

ModelViewer::~ModelViewer() = default;

void ModelViewer::loadProperties(const rapidjson::Value& inObj) {
    mMeshManager->loadProperties(inObj);
    mLight->loadProperties(inObj);
}

void ModelViewer::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mLight->saveProperties(alloc, inObj);
}

void ModelViewer::initialize(
    const IEngineModeGetter* engineModeGetter,
    const ICurrentSelectTextureGetter* assetsTextureGetter,
    ICallbackSelectAssetsTexture* callbackSelectAssetsTexture,
    IEngineFunctionChanger* engineFunctionChanger,
    IEngineManagingClassGetter* engineManagingClassGetter
) {
    mEngineModeGetter = engineModeGetter;
    mAssetsTextureGetter = assetsTextureGetter;

    mMeshManager->initialize();
    mModelViewCamera->initialize();
    mPlane->initialize(*mMeshManager, engineFunctionChanger);

    callbackSelectAssetsTexture->callbackSelectTexture([&] { onSelectAssetsTexture(); });
    engineFunctionChanger->callbackChangeMode([&](EngineMode mode) { onChangeMode(mode); });

    auto inspectorPosX = engineManagingClassGetter->debug().getDebugLayer().inspector()->getInspectorPositionX();
    mGuiSizeX = inspectorPosX - Window::width();

}

void ModelViewer::update(EngineMode mode) {
    if (mode == EngineMode::MODEL_VIEWER) {
        if (mTarget.first) {
            mTarget.first->update();
            mTarget.first->lateUpdate();
        }
        mPlane->update();

        mModelViewCamera->update();
        mAnimationViewer->update();

        saveModel();
    }
}

void ModelViewer::setTarget(
    const GameObjectPtr& targetObj,
    const MeshRendererPtr& targetMesh
) {
    if (mTarget.second) {
        mMeshManager->erase(mTarget.second);
    }

    mTarget = std::make_pair(targetObj, targetMesh);
    mMeshManager->add(targetMesh, true);

    auto mesh = targetMesh->getMeshComponent().getMesh();
    mModelViewCamera->onChangeModel(*mesh);
    mAnimationViewer->onChangeModel(*targetObj);
    mPlane->onChangeModel(*mesh);
}

void ModelViewer::draw(EngineMode mode) const {
    if (mode == EngineMode::MODEL_VIEWER) {
        //パラメータ調整用GUI描画
        drawGUI();

        //モデル描画
        const auto& camera = mModelViewCamera->getCamera();
        mMeshManager->draw(
            camera.getView(),
            camera.getProjection(),
            camera.getPosition(),
            mLight->getDirection(),
            mLight->getColor()
        );
    }
}

void ModelViewer::drawGUI() const {
    //マウス位置を補正する
    const auto& mousePos = Input::mouse().getMousePosition();
    ImGui::GetIO().MousePos = ImVec2(mousePos.x, mousePos.y);
    //ウィンドウ位置を固定
    ImGui::SetNextWindowPos(ImVec2(Window::width(), 0.f), ImGuiCond_Always);
    //ウィンドウサイズを固定
    ImGui::SetNextWindowSize(ImVec2(mGuiSizeX, Window::debugHeight()), ImGuiCond_Always);

    ImGui::Begin("ModelViewer");

    mLight->drawGUI();
    ImGui::Separator(); //区切り線
    mPlane->drawGUI();

    ImGui::End();
}

void ModelViewer::saveModel() {
    const auto& keyboard = Input::keyboard();
    bool pressedS = keyboard.getKeyDown(KeyCode::S);
    bool pressedLeftControl = keyboard.getKey(KeyCode::LeftControl);
    if (pressedLeftControl && pressedS) {
        if (!mTarget.first) {
            return;
        }

        SaveModelViewerCollider::save(*mTarget.first);
    }
}

void ModelViewer::onChangeMode(EngineMode mode) {
    bool isModelViewer = (mode == EngineMode::MODEL_VIEWER);
    if (isModelViewer) {
        mMeshManager->registerThisToMeshRenderer();
    }

    if (mTarget.first) {
        mTarget.first->setActive(isModelViewer);
    }
}

void ModelViewer::onSelectAssetsTexture() {
    if (mEngineModeGetter->getMode() != EngineMode::MODEL_VIEWER) {
        return;
    }
    if (!mAssetsTextureGetter->selectedTexture()) {
        return;
    }

    const auto& newTarget = std::make_shared<GameObject>();
    const auto& newMesh = Component::addComponent<MeshComponent>(*newTarget, "MeshComponent");
    newMesh->createMeshFromFilePath(mAssetsTextureGetter->getCurrentSelectTexture().getTexture().getFilePath());
    const auto& meshRenderer = newMesh->getComponent<MeshRenderer>();

    setTarget(newTarget, meshRenderer);
}
