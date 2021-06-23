#pragma once

#include "../EngineMode.h"
#include "../IEngineFunctionChanger.h"
#include "../IEngineManagingClassGetter.h"
#include "../IEngineModeGetter.h"
#include "../AssetsRenderer/ICallbackSelectAssetsTexture.h"
#include "../AssetsRenderer/ICurrentSelectTextureGetter.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <utility>

class GameObject;
class MeshManager;
class MeshRenderer;
class ModelViewCamera;
class AnimationViewer;
class ModelViewerLight;
class ModelViewerPlane;

class ModelViewer {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
    using GameObjectMeshRendererPair = std::pair<GameObjectPtr, MeshRendererPtr>;

public:
    ModelViewer();
    ~ModelViewer();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj);

    //初期化
    void initialize(
        const IEngineModeGetter* engineModeGetter,
        const ICurrentSelectTextureGetter* assetsTextureGetter,
        ICallbackSelectAssetsTexture* callbackSelectAssetsTexture,
        IEngineFunctionChanger* engineFunctionChanger,
        IEngineManagingClassGetter* engineManagingClassGetter
    );

    //毎フレーム更新
    void update(EngineMode mode);

    //モデル対象を設定する
    void setTarget(
        const GameObjectPtr& targetObj,
        const MeshRendererPtr& targetMesh
    );

    //モデルを描画する
    void draw(EngineMode mode) const;

private:
    ModelViewer(const ModelViewer&) = delete;
    ModelViewer& operator=(const ModelViewer&) = delete;

    //モデルビューア独自のGUIを描画する
    void drawGUI() const;
    //現在のモデル状態を保存する
    void saveModel();
    //エンジンモード変更時
    void onChangeMode(EngineMode mode);
    //アセットテクスチャが選択されたとき
    void onSelectAssetsTexture();

private:
    const IEngineModeGetter* mEngineModeGetter;
    const ICurrentSelectTextureGetter* mAssetsTextureGetter;
    GameObjectMeshRendererPair mTarget;
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<ModelViewerPlane> mPlane;
    std::unique_ptr<ModelViewCamera> mModelViewCamera;
    std::unique_ptr<AnimationViewer> mAnimationViewer;
    std::unique_ptr<ModelViewerLight> mLight;
    float mGuiSizeX;
};
