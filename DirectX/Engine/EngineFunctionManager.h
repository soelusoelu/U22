﻿#pragma once

#include "EngineMode.h"
#include "IEngineManagingClassGetter.h"
#include "IEngineModeGetter.h"
#include "../GameObject/IGameObjectsGetter.h"
#include "../Math/Math.h"
#include "../Mesh/IMeshesGetter.h"
#include "../System/FpsCounter/IFpsGetter.h"
#include "../Utility/FileMode.h"
#include <memory>
#include <string>
#include <rapidjson/document.h>

class Camera;
class DirectionalLight;
class Renderer;
class Pause;
class EngineFuctionChanger;
class SceneMeshOperator;
class ModelViewer;

//エンジン機能統括クラス
class EngineFunctionManager
    : public IEngineManagingClassGetter
{
public:
    EngineFunctionManager();
    ~EngineFunctionManager();

    virtual IEngineFunctionChanger& getModeChanger() const override;
    virtual DebugManager& debug() const override;
    virtual IPause& pause() const override;
    virtual AssetsRenderTextureManager& getAssetsRenderTextureManager() const override;
    virtual MapEditorMeshManager& getMapEditorMeshManager() const override;

    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);

    //初期化
    void initialize(
        const std::shared_ptr<Camera>& camera,
        const IEngineModeGetter* engineModeGetter,
        const IGameObjectsGetter* gameObjctsGetter,
        const IMeshesGetter* meshesGetter,
        const IFpsGetter* fpsGetter
    );

    //アップデート前処理
    void preUpdateProcess();
    //毎フレーム更新
    void update(EngineMode mode);

    //2D関連の描画
    void draw(
        EngineMode mode,
        const Renderer& renderer,
        Matrix4& proj
    ) const;

    //3D関連の描画
    void draw3D(
        EngineMode mode,
        const Renderer& renderer,
        const Camera& camera,
        const DirectionalLight& dirLight
    ) const;

private:
    EngineFunctionManager(const EngineFunctionManager&) = delete;
    EngineFunctionManager& operator=(const EngineFunctionManager&) = delete;

private:
    std::unique_ptr<DebugManager> mDebugManager;
    std::unique_ptr<Pause> mPause;
    std::unique_ptr<EngineFuctionChanger> mFunctionChanger;
    std::unique_ptr<MapEditorMeshManager> mMapEditor;
    std::unique_ptr<AssetsRenderTextureManager> mAssetsRenderTextureManager;
    std::unique_ptr<SceneMeshOperator> mSceneMeshOperator;
    std::unique_ptr<ModelViewer> mModelViewer;
};
