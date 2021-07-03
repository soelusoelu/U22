#include "EngineFuctionChanger.h"
#include "../Device/Button.h"
#include "../Input/Input.h"
#include "../Utility/JsonHelper.h"

EngineFuctionChanger::EngineFuctionChanger()
    : mStartRenderPosition()
    , mSpriteSpace(0.f)
{
}

EngineFuctionChanger::~EngineFuctionChanger() = default;

void EngineFuctionChanger::changeMode(EngineMode mode) {
    mCallbackChangeMode(mode);
}

void EngineFuctionChanger::callbackChangeMode(const std::function<void(EngineMode)>& f) {
    mCallbackChangeMode += f;
}

void EngineFuctionChanger::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::SAVE) {
        rapidjson::Value props(rapidjson::kObjectType);
        JsonHelper::setVector2(mStartRenderPosition, "startRenderPosition", props, alloc);
        JsonHelper::setStringArray(mSpritesFilePath, "spritesFilePath", props, alloc);
        JsonHelper::setFloat(mSpriteSpace, "spriteSpace", props, alloc);

        inObj.AddMember("engineFuctionChanger", props, alloc);
    } else {
        const auto& efcObj = inObj["engineFuctionChanger"];
        JsonHelper::getVector2(mStartRenderPosition, "startRenderPosition", efcObj);
        JsonHelper::getStringArray(mSpritesFilePath, "spritesFilePath", efcObj);
        JsonHelper::getFloat(mSpriteSpace, "spriteSpace", efcObj);
    }
}

void EngineFuctionChanger::initialize() {
    for (size_t i = 0; i < mSpritesFilePath.size(); ++i) {
        mSpritesButton.emplace_back(std::make_unique<SpriteButton>(nullptr, mSpritesFilePath[i], mStartRenderPosition + Vector2::right * (SPRITE_WIDTH + mSpriteSpace) * i));
    }
}

void EngineFuctionChanger::update() {
    const auto& mousePos = Input::mouse().getMousePosition();
    for (size_t i = 0; i < mSpritesButton.size(); ++i) {
        bool result = mSpritesButton[i]->clickButton(mousePos);
        if (result) {
            mCallbackChangeMode(static_cast<EngineMode>(i));
            return;
        }
    }
}
