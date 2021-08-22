#pragma once

#include "../../Component.h"
#include "../../Engine/EngineComponentAlias.h"
#include <memory>
#include <vector>

class SpriteComponent;

class Clear
    : public Component
{
public:
    Clear();
    ~Clear();
    Clear(const Clear&) = delete;
    Clear& operator=(const Clear&) = delete;

    virtual void start() override;

    void setActive(bool value);

private:
    SpritePtrArray mSprites;
};
