#include "EngineFuctionChanger.h"
#include "../Device/Button.h"
#include "../Input/Input.h"
#include "../Utility/LevelLoader.h"

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

void EngineFuctionChanger::loadProperties(const rapidjson::Value& inObj) {
    const auto& efcObj = inObj["engineFuctionChanger"];
    if (efcObj.IsObject()) {
        JsonHelper::getVector2(efcObj, "startRenderPosition", mStartRenderPosition);
        JsonHelper::getStringArray(efcObj, "spritesFilePath", mSpritesFilePath);
        JsonHelper::getFloat(efcObj, "spriteSpace", mSpriteSpace);
    }
}

void EngineFuctionChanger::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setVector2(alloc, props, "startRenderPosition", mStartRenderPosition);
    JsonHelper::setStringArray(alloc, props, "spritesFilePath", mSpritesFilePath);
    JsonHelper::setFloat(alloc, props, "spriteSpace", mSpriteSpace);

    inObj.AddMember("engineFuctionChanger", props, alloc);
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
