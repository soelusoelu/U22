#pragma once

#include "IPause.h"
#include "../IEngineFunctionChanger.h"
#include "../../Math/Math.h"
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
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
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
