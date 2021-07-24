#include "LineInstancingDrawer.h"
#include "LineRenderer3D.h"
#include "../../../../System/AssetsManager.h"
#include "../../../../System/Shader/DataTransfer.h"
#include "../../../../System/Shader/ShaderBinder.h"
#include "../../../../Transform/Transform3D.h"

LineInstancingDrawer::LineInstancingDrawer()
    : mInputBuffer(nullptr)
    , mShaderID(-1)
{
}

LineInstancingDrawer::~LineInstancingDrawer() = default;

void LineInstancingDrawer::initialize() {
    //シェーダーIDの取得
    mShaderID = AssetsManager::instance().createShader("Line3DInstancing.hlsl");

    //入力バッファの作成
    BufferDesc bd{};
    bd.size = sizeof(LineConstantBuffer) * MAX_INSTANCE;
    bd.usage = Usage::DYNAMIC;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);
    bd.cpuAccessFlags = static_cast<unsigned>(BufferCPUAccessFlag::WRITE);

    mInputBuffer = std::make_unique<VertexBuffer>(bd);
}

void LineInstancingDrawer::add(const Vector3& p1, const Vector3& p2, const Vector3& color) {
    auto mat = Matrix4::createScale(p2 - p1);
    mat *= Matrix4::createTranslation(p1);

    //スプライトから描画に必要なデータを抜き出す
    LineConstantBuffer lcb{};
    lcb.wp = mat;
    lcb.color = Vector4(color, 1.f);

    //新しいインスタンスデータを登録
    mInstancingData.emplace_back(lcb);
}

void LineInstancingDrawer::instancingDraw(const Matrix4& viewProj) {
    if (mInstancingData.empty()) {
        return;
    }

    //全ての描画データに射影行列を掛ける
    for (auto&& data : mInstancingData) {
        data.wp *= viewProj;
    }

    //インスタンシング用頂点バッファを登録する
    auto lineBuf = LineRenderer::vertexBuffer;
    ID3D11Buffer* buffers[NUM_VERTEX_BUFFER] = { lineBuf->buffer(), mInputBuffer->buffer() };
    unsigned strides[NUM_VERTEX_BUFFER] = { lineBuf->desc().oneSize, sizeof(LineConstantBuffer) };
    lineBuf->setVertexBuffer(NUM_VERTEX_BUFFER, buffers, strides);
    //インデックスバッファを登録する
    LineRenderer::indexBuffer->setIndexBuffer();

    //シェーダーを登録する
    ShaderBinder::bind(mShaderID);

    //シェーダーにデータ転送
    DataTransfer::transferData(
        *mInputBuffer,
        mInstancingData.data(),
        mInstancingData.size()
    );

    //描画
    MyDirectX::DirectX::instance().drawIndexedInstanced(2, mInstancingData.size());
}

void LineInstancingDrawer::clear() {
    mInstancingData.clear();
}
