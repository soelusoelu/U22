#include "LineRenderer.h"
#include "../../../../DirectX/DirectXInclude.h"
#include "../../../../System/GlobalFunction.h"

LineRenderer::LineRenderer() = default;

LineRenderer::~LineRenderer() = default;

void LineRenderer::finalize() {
    safeDelete(vertexBuffer);
    safeDelete(indexBuffer);
}

void LineRenderer::draw(const Matrix4& proj) const {
    //描画共通処理は最初に済ませる
    //バーテックスバッファーを登録
    vertexBuffer->setVertexBuffer();
    //インデックスバッファーを登録
    indexBuffer->setIndexBuffer();

    //描画指令を出す
    drawLines(proj);
}

void LineRenderer::initialize() {
    //バーテックスバッファの作成
    if (!vertexBuffer) {
        createVertexBuffer();
    }
    //インデックスバッファの作成
    if (!indexBuffer) {
        createIndexBuffer();
    }

    //子クラスのシェーダーを作成する
    createShader();
}

void LineRenderer::createVertexBuffer() {
    BufferDesc bd;
    bd.oneSize = getParamSize();
    bd.size = bd.oneSize * 2; //頂点が2つだから
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);

    SubResourceDesc sub;
    sub.data = getVertexData();

    vertexBuffer = new VertexBuffer(bd, &sub);
}

void LineRenderer::createIndexBuffer() {
    static constexpr unsigned short indices[] = {
        0, 1
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::INDEX);

    SubResourceDesc sub;
    sub.data = indices;

    indexBuffer = new IndexBuffer(bd, sub);
}
