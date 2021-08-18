#pragma once

#include "IEngineManagingClassGetter.h"
#include "../Device/FileOperator.h"
#include "../GameObject/IGameObjectsGetter.h"
#include "../Math/Math.h"
#include "../Mesh/IMeshesGetter.h"
#include "../System/FpsCounter/IFpsGetter.h"
#include <memory>
#include <string>

class Camera;
class DirectionalLight;
class Renderer;
class Pause;

//エンジン機能統括クラス
class EngineFunctionManager
    : public FileOperator
    , public IEngineManagingClassGetter
{
public:
    EngineFunctionManager();
    ~EngineFunctionManager();

    virtual DebugManager& debug() const override;
    virtual IPause& pause() const override;

    //初期化
    void initialize(
        const IGameObjectsGetter* gameObjctsGetter,
        const IFpsGetter* fpsGetter
    );

    //アップデート前処理
    void preUpdateProcess();
    //毎フレーム更新
    void update();

    //2D関連の描画
    void draw2D(const Renderer& renderer, Matrix4& proj) const;
    //2Dデバッグ関連の描画
    void drawDebug2D(Matrix4& proj) const;

    //3D関連の描画
    void draw3D(
        const Renderer& renderer,
        const Camera& camera
    ) const;

private:
    EngineFunctionManager(const EngineFunctionManager&) = delete;
    EngineFunctionManager& operator=(const EngineFunctionManager&) = delete;

    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::unique_ptr<DebugManager> mDebugManager;
    std::unique_ptr<Pause> mPause;
};
