#pragma once

#include "IAddAssets.h"
#include "../EngineMode.h"
#include "../IEngineFunctionChanger.h"
#include "../../Math/Math.h"
#include "../../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class SpriteButton;

//メッシュを描画したテクスチャ追加クラス
class AssetsRenderTextureAdder {
public:
    AssetsRenderTextureAdder();
    ~AssetsRenderTextureAdder();
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    void initialize(IAddAssets* adder, IEngineFunctionChanger& changer);
    void update();

private:
    AssetsRenderTextureAdder(const AssetsRenderTextureAdder&) = delete;
    AssetsRenderTextureAdder& operator=(const AssetsRenderTextureAdder&) = delete;

    void onClickButton();
    void onChangeMode(EngineMode mode);

private:
    std::unique_ptr<SpriteButton> mButton;
    IAddAssets* mAssetsAdder;
    std::string mSpriteFilePath;
    Vector2 mRenderPosition;
};
