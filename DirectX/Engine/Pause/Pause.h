#pragma once

#include "IPause.h"
#include "../../Device/FileOperator.h"
#include "../../Math/Math.h"
#include <memory>
#include <string>

class SpriteButton;

class Pause
    : public FileOperator
    , public IPause
{
public:
    Pause();
    ~Pause();
    virtual bool isPausing() const override;

    void initialize();
    void update();

private:
    Pause(const Pause&) = delete;
    Pause& operator=(const Pause&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    void onModeChange();

private:
    std::unique_ptr<SpriteButton> mButton;
    std::string mFileName;
    Vector2 mOffset;
    bool mIsPausing;
};
