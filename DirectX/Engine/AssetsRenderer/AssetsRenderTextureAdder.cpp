#include "AssetsRenderTextureAdder.h"
#include "../../Device/Button.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/JsonHelper.h"

AssetsRenderTextureAdder::AssetsRenderTextureAdder()
    : mButton(nullptr)
    , mAssetsAdder(nullptr)
    , mRenderPosition()
{
}

AssetsRenderTextureAdder::~AssetsRenderTextureAdder() = default;

void AssetsRenderTextureAdder::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::SAVE) {
        rapidjson::Value props(rapidjson::kObjectType);
        JsonHelper::setVector2(mRenderPosition, "renderPosition", props, alloc);
        JsonHelper::setString(mSpriteFilePath, "spriteButtonFilePath", props, alloc);

        inObj.AddMember("assetsRenderTextureAdder", props, alloc);
    } else {
        const auto& artaObj = inObj["assetsRenderTextureAdder"];
        JsonHelper::getVector2(mRenderPosition, "renderPosition", artaObj);
        JsonHelper::getString(mSpriteFilePath, "spriteButtonFilePath", artaObj);
    }
}

void AssetsRenderTextureAdder::initialize(IAddAssets* adder, IEngineFunctionChanger& changer) {
    mAssetsAdder = adder;
    mButton = std::make_unique<SpriteButton>([&] { onClickButton(); }, mSpriteFilePath, mRenderPosition);
    changer.callbackChangeMode([&](EngineMode mode) { onChangeMode(mode); });
}

void AssetsRenderTextureAdder::update() {
    mButton->clickButton(Input::mouse().getMousePosition());
}

void AssetsRenderTextureAdder::onClickButton() {
    if (std::string outFilePath, outFileName; FileUtil::openFileDialog(outFilePath, outFileName)) {
        //絶対パスからアセットディレクトリ部分を抜き出す
        const auto& assetsDir = FileUtil::getAssetsFromAbsolutePath(outFilePath);
        mAssetsAdder->add(assetsDir);
    }
}

void AssetsRenderTextureAdder::onChangeMode(EngineMode mode) {
    bool isActive = (mode == EngineMode::MAP_EDITOR || mode == EngineMode::MODEL_VIEWER);
    if (mButton->getActive() != isActive) {
        mButton->setActive(isActive);
    }
}
