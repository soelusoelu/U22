#pragma once

#include "IPause.h"
#include "../IEngineFunctionChanger.h"
#include "../../Math/Math.h"
#include "../../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class SpriteButton;

class Pause
    : public IPause
{
public:
    Pause();
    ~Pause();
    virtual bool isPausing() const override;
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    void initialize(IEngineFunctionChanger* modeChanger);
    void update();

private:
    Pause(const Pause&) = delete;
    Pause& operator=(const Pause&) = delete;

    void onModeChange(EngineMode mode);

private:
    std::unique_ptr<SpriteButton> mButton;
    std::string mFileName;
    Vector2 mOffset;
    bool mIsPausing;
};
