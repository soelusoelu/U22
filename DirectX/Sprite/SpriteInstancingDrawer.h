#pragma once

#include "../DirectX/DirectXInclude.h"
#include "../Math/Math.h"
#include "../System/Shader/ConstantBuffers.h"
#include <vector>

class Sprite;

class SpriteInstancingDrawer {
public:
    SpriteInstancingDrawer();
    ~SpriteInstancingDrawer();
    //インスタンスデータを登録する
    void add(const Sprite& sprite, const Matrix4& proj);
    //インスタンスを描画する
    void instancingDraw(const Sprite& sprite, const Matrix4& proj) const;
    void clear();

private:
    SpriteInstancingDrawer(const SpriteInstancingDrawer&) = delete;
    SpriteInstancingDrawer& operator=(const SpriteInstancingDrawer&) = delete;

private:
    std::vector<TextureConstantBuffer> mInstancingData;
    std::unique_ptr<VertexBuffer> mInputBuffer;
    static constexpr int MAX_INSTANCE = 1024;
};
