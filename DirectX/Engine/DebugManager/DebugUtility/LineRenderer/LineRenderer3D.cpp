#include "LineRenderer3D.h"
#include "../../../../DirectX/DirectXInclude.h"
#include "../../../../System/AssetsManager.h"
#include "../../../../System/Shader/ConstantBuffers.h"
#include "../../../../System/Shader/DataTransfer.h"
#include "../../../../System/Shader/ShaderBinder.h"
#include "../../../../Transform/Transform3D.h"
#include <vector>

LineRenderer3D::LineRenderer3D() :
    LineRenderer(),
    mShaderID(-1),
    mTransform(std::make_unique<Transform3D>()) {
}

LineRenderer3D::~LineRenderer3D() = default;

void LineRenderer3D::clear() {
    mLines.clear();
}

void LineRenderer3D::renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color) {
    mLines.emplace_back(Line3DParam{ p1, p2, color });
}

unsigned LineRenderer3D::getParamSize() const {
    return sizeof(Line3DVertex);
}

const void* LineRenderer3D::getVertexData() const {
    static const Line3DVertex vert[] = {
        Vector3::zero, Vector3::one
    };
    return vert;
}

void LineRenderer3D::createShader() {
    //シェーダー作成
    mShaderID = AssetsManager::instance().createShader("Line3D.hlsl");
}

void LineRenderer3D::drawLines(const Matrix4& proj) const {
    //シェーダーを登録
    ShaderBinder::bind(mShaderID);

    for (const auto& line : mLines) {
        drawLine(line, proj);
    }
}

void LineRenderer3D::drawLine(const Line3DParam& param, const Matrix4& proj) const {
    //パラメータからワールド行列を計算する
    mTransform->setScale(param.p2 - param.p1);
    mTransform->setPosition(param.p1);
    mTransform->computeMatrix();

    //シェーダーに値を渡す
    LineConstantBuffer cb;
    cb.wp = mTransform->getWorldTransform() * proj;
    cb.color = Vector4(param.color, 1.f);

    //シェーダーにデータ転送
    DataTransfer::transferConstantBuffer(mShaderID, &cb);

    //描画
    MyDirectX::DirectX::instance().drawIndexed(2);
}
