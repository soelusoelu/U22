﻿#pragma once

#include "../../Component.h"
#include "../../../Mesh/IAnimation.h"
#include "../../../Mesh/IMesh.h"
#include <memory>
#include <unordered_map>
#include <utility>

class Shader;

//メッシュのシェーダーを扱うクラス
class MeshShader : public Component {
public:
    MeshShader();
    ~MeshShader();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawInspector() override;

    //シェーダーを使用する準備をする
    void preSet() const;
    //シェーダーにデータを転送する
    void transferData();

    //メッシュ共通の値を設定する
    void setCommonValue(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;
    //マテリアルデータを設定する
    void setMaterialData(unsigned materialIndex, unsigned constantBufferIndex = 1) const;
    //全メッシュに共通する値を設定する
    //データが生きている必要あり
    void setTransferData(const void* data, unsigned size, unsigned constantBufferIndex);

    //各種インターフェースを設定する
    void setInterface(const IMesh* mesh, const IAnimation* anim);
    //デフォルトのシェーダーに変更する
    void setDefaultShader();

private:
    MeshShader(const MeshShader&) = delete;
    MeshShader& operator=(const MeshShader&) = delete;

private:
    struct TransferData {
        const void* data;
        unsigned size;
    };

    const IMesh* mMesh;
    const IAnimation* mAnimation;
    std::shared_ptr<Shader> mShader;
    std::unordered_map<unsigned, TransferData> mTransferDataMap;
};
