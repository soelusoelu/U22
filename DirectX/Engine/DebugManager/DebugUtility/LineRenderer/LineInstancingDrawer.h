#pragma once

#include "../../../../DirectX/DirectXInclude.h"
#include "../../../../Math/Math.h"
#include "../../../../System/Shader/ConstantBuffers.h"
#include <memory>
#include <vector>

class LineInstancingDrawer {
public:
    LineInstancingDrawer();
    ~LineInstancingDrawer();
    //初期化
    void initialize();
    //インスタンスデータを登録する
    void add(const Vector3& p1, const Vector3& p2, const Vector3& color);
    //インスタンスを描画する
    void instancingDraw(const Matrix4& viewProj);
    //全バッファ削除
    void clear();

private:
    LineInstancingDrawer(const LineInstancingDrawer&) = delete;
    LineInstancingDrawer& operator=(const LineInstancingDrawer&) = delete;

private:
    //インスタンシングデータ
    std::vector<LineConstantBuffer> mInstancingData;
    //インスタンスごとの固有データ用バッファ
    std::unique_ptr<VertexBuffer> mInputBuffer;
    //シェーダーID
    int mShaderID;

    //一度に描画する最大数
    static constexpr int MAX_INSTANCE = 2048;
    //頂点バッファ数
    static constexpr int NUM_VERTEX_BUFFER = 2;
};
