﻿#pragma once

#include "IModelViewerCallback.h"
#include "ModelViewerMode.h"
#include "../EngineMode.h"
#include "../IEngineFunctionChanger.h"
#include "../IEngineManagingClassGetter.h"
#include "../IEngineModeGetter.h"
#include "../AssetsRenderer/ICallbackSelectAssetsTexture.h"
#include "../AssetsRenderer/ICurrentSelectTextureGetter.h"
#include "../../Device/Function.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <utility>

class GameObject;
class Renderer;
class MeshManager;
class LineRenderer3D;
class MeshRenderer;
class ModelViewCamera;
class AnimationViewer;
class ModelViewerLight;
class ModelViewerPlane;
class ModelViewerColliderManager;

class ModelViewer
    : public IModelViewerCallback
{
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
    using GameObjectMeshRendererPair = std::pair<GameObjectPtr, MeshRendererPtr>;

public:
    ModelViewer();
    ~ModelViewer();

    virtual void callbackModeChange(const std::function<void(ModelViewerMode)>& f) override;
    virtual void callbackModelChange(const std::function<void(GameObject&)>& f) override;

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
    void draw(EngineMode mode, const Renderer& renderer) const;

private:
    ModelViewer(const ModelViewer&) = delete;
    ModelViewer& operator=(const ModelViewer&) = delete;

    //モデルビューア独自のGUIを描画する
    void drawGUI() const;
    //モードを変更する
    void changeMode(ModelViewerMode mode);
    //現在のモデル状態を保存する
    void saveModel();
    //エンジンモード変更時
    void onChangeMode(EngineMode mode);
    //アセットテクスチャが選択されたとき
    void onSelectAssetsTexture();

private:
    const IEngineModeGetter* mEngineModeGetter;
    const ICurrentSelectTextureGetter* mAssetsTextureGetter;
    //モデルビューア独自の描画クラス
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<LineRenderer3D> mLineRenderer3D;
    //描画ターゲット
    GameObjectMeshRendererPair mTarget;
    //床
    std::unique_ptr<ModelViewerPlane> mPlane;
    //モデルビューア用カメラ
    std::unique_ptr<ModelViewCamera> mModelViewCamera;
    //モデルビューア用ライト
    std::unique_ptr<ModelViewerLight> mLight;
    //アニメーション制御クラス
    std::unique_ptr<AnimationViewer> mAnimationViewer;
    //当たり判定管理クラス
    std::unique_ptr<ModelViewerColliderManager> mColliderManager;
    //各種コールバック
    Function<void(ModelViewerMode)> mCallbackModeChange;
    Function<void(GameObject&)> mCallbackModelChange;
    //現在の描画モード
    ModelViewerMode mMode;
    //調整用GUIの横サイズ
    float mGuiSizeX;
};
