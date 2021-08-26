#pragma once

#include "../../Component.h"
#include "../../Engine/EngineComponentAlias.h"

class CrossHair
    : public Component
{
public:
    CrossHair();
    ~CrossHair();
    CrossHair(const CrossHair&) = delete;
    CrossHair& operator=(const CrossHair&) = delete;

    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    SpritePtrArray mSprites;
    float mOffset;
    float mLength;

    inline static constexpr int CROSS_HAIR_COUNT = 4;
};
