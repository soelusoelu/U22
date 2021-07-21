﻿#pragma once

#include "IModelViewerCallback.h"
#include "ModelViewerMode.h"
#include "../../Math/Math.h"
#include <array>
#include <memory>
#include <vector>

class GameObject;
class MeshComponent;
class SkinMeshComponent;
class LineRenderer3D;
class SimpleCamera;
class OBBCollider;
class ColliderOptionGUI;

class ModelViewerColliderManager {
    using OBBColliderPtr = std::shared_ptr<OBBCollider>;

public:
    ModelViewerColliderManager();
    ~ModelViewerColliderManager();
    void initialize(IModelViewerCallback* callback);
    void update(LineRenderer3D& line, const SimpleCamera& camera);
    void drawGUI();

private:
    ModelViewerColliderManager(const ModelViewerColliderManager&) = delete;
    ModelViewerColliderManager& operator=(const ModelViewerColliderManager&) = delete;

    //Tポーズ状態のボーンを描画する
    void drawTPoseBone(LineRenderer3D& line) const;
    //ボーンの数だけOBBを作成する
    void createObbCollider();
    //OBBをマウスで選択する
    void selectObb(const SimpleCamera& camera);
    //アニメーション付きのモデルか
    bool isAnimation() const;
    //コールバック
    void onChangeModel(const GameObject& newModel);
    void onModeChange(ModelViewerMode mode);

private:
    std::unique_ptr<ColliderOptionGUI> mOptionGui;
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<SkinMeshComponent> mSkinMesh;
    std::vector<OBBColliderPtr> mObbColliders;
    int mSelectedObbNo;
    bool mIsDrawBone;

    inline static constexpr int INVALID_NO = -1;
    inline static const std::array COLORS = {
        ColorPalette::red, ColorPalette::blue, ColorPalette::yellow, ColorPalette::green,
        ColorPalette::lightBlue, ColorPalette::lightGreen, ColorPalette::lightPink, ColorPalette::lightYellow
    };
};
