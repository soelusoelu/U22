#pragma once

#include "EngineMode.h"
#include "IEngineFunctionChanger.h"
#include "../Device/Function.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <vector>

class SpriteButton;

class EngineFuctionChanger
    : public IEngineFunctionChanger
{
    using SpriteButtonPtr = std::unique_ptr<SpriteButton>;
    using SpriteButtonPtrArray = std::vector<SpriteButtonPtr>;
    using SpriteFilePathArray = std::vector<std::string>;

public:
    EngineFuctionChanger();
    ~EngineFuctionChanger();
    virtual void callbackChangeMode(const std::function<void(EngineMode)>& f) override;
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void initialize();
    void update();
    void draw(const Matrix4& proj) const;

private:
    EngineFuctionChanger(const EngineFuctionChanger&) = delete;
    EngineFuctionChanger& operator=(const EngineFuctionChanger&) = delete;

private:
    SpriteButtonPtrArray mSpritesButton;
    SpriteFilePathArray mSpritesFilePath;
    Function<void(EngineMode)> mCallbackChangeMode;
    Vector2 mStartRenderPosition;
    float mSpriteSpace;

    static constexpr float SPRITE_WIDTH = 64.f;
};
