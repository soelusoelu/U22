﻿#pragma once

#include "IModelViewerCallback.h"
#include "ModelViewerMode.h"
#include "../../Mesh/IAnimation.h"
#include <memory>

class GameObject;
class SkinMeshComponent;
class LineRenderer3D;

class ModelViewerColliderManager {
public:
    ModelViewerColliderManager();
    ~ModelViewerColliderManager();
    void initialize(IModelViewerCallback* callback);
    void update(LineRenderer3D& line);

private:
    ModelViewerColliderManager(const ModelViewerColliderManager&) = delete;
    ModelViewerColliderManager& operator=(const ModelViewerColliderManager&) = delete;

    //Tポーズ状態のボーンを描画する
    void drawTPoseBone(LineRenderer3D& line) const;
    void onChangeModel(const GameObject& newModel);
    void onModeChange(ModelViewerMode mode);

private:
    std::shared_ptr<SkinMeshComponent> mSkinMesh;
    IAnimation* mAnimation;

    static const Vector3 COLORS[];
};