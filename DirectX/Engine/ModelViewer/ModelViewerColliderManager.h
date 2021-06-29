#pragma once

#include "IModelViewerCallback.h"
#include "ModelViewerMode.h"
#include "../../Math/Math.h"
#include <memory>
#include <vector>

class GameObject;
class MeshComponent;
class SkinMeshComponent;
class LineRenderer3D;
class SimpleCamera;
class OBBCollider;

class ModelViewerColliderManager {
public:
    ModelViewerColliderManager();
    ~ModelViewerColliderManager();
    void initialize(IModelViewerCallback* callback);
    void update(LineRenderer3D& line, const SimpleCamera& camera);

private:
    ModelViewerColliderManager(const ModelViewerColliderManager&) = delete;
    ModelViewerColliderManager& operator=(const ModelViewerColliderManager&) = delete;

    //Tポーズ状態のボーンを描画する
    void drawTPoseBone(LineRenderer3D& line) const;
    //ボーンの数だけOBBを作成する
    void createObbCollider();
    //アニメーション付きのモデルか
    bool isAnimation() const;
    void onChangeModel(const GameObject& newModel);
    void onModeChange(ModelViewerMode mode);

private:
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<SkinMeshComponent> mSkinMesh;
    std::vector<std::shared_ptr<OBBCollider>> mObbColliders;

    static const Vector3 COLORS[];
};
