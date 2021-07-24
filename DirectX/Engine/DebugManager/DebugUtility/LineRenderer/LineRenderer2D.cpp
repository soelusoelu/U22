﻿#include "LineRenderer2D.h"
#include "../../../../DirectX/DirectXInclude.h"
#include "../../../../System/AssetsManager.h"
#include "../../../../System/Shader/ConstantBuffers.h"
#include "../../../../System/Shader/DataTransfer.h"
#include "../../../../System/Shader/ShaderBinder.h"
#include "../../../../Transform/Transform2D.h"
#include <vector>

LineRenderer2D::LineRenderer2D()
    : LineRenderer()
    , mShaderID(-1)
    , mTransform(std::make_unique<Transform2D>())
{
    //ラインのサイズはバーテックスバッファに合わせる
    mTransform->setSize(Vector2::one);
}

LineRenderer2D::~LineRenderer2D() = default;

void LineRenderer2D::clear() {
    mLines.clear();
}

void LineRenderer2D::renderLine(const Vector2& p1, const Vector2& p2, const Vector3& color) {
    mLines.emplace_back(Line2DParam{ p1, p2, color });
}

unsigned LineRenderer2D::getParamSize() const {
    return sizeof(Line2DVertex);
}

const void* LineRenderer2D::getVertexData() const {
    static const Line2DVertex vert[] = {
        Vector2::zero, Vector2::one
    };
    return vert;
}

void LineRenderer2D::createShader() {
    //シェーダー作成
    mShaderID = AssetsManager::instance().createShader("Line2D.hlsl");
}

void LineRenderer2D::drawLines(const Matrix4& proj) const {
    //シェーダーを登録
    ShaderBinder::bind(mShaderID);

    for (const auto& line : mLines) {
        drawLine(line, proj);
    }
}

void LineRenderer2D::drawLine(const Line2DParam& param, const Matrix4& proj) const {
    //パラメータからワールド行列を計算する
    mTransform->setScale(param.p2 - param.p1);
    mTransform->setPosition(param.p1);
    mTransform->computeWorldTransform();

    //シェーダーに値を渡す
    LineConstantBuffer cb;
    cb.wp = mTransform->getWorldTransform() * proj;
    cb.color = Vector4(param.color, 1.f);

    //データ転送
    DataTransfer::transferConstantBuffer(mShaderID, &cb);

    //描画
    MyDirectX::DirectX::instance().drawIndexed(2);
}
