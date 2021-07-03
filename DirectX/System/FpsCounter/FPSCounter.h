﻿#pragma once

#include "IFpsGetter.h"
#include "../SystemInclude.h"
#include "../../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>

class Time;

class FPSCounter : public IFpsGetter {
public:
    FPSCounter();
    ~FPSCounter();
    virtual float getFps() const override;

    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    //設定したフレームレートに固定する
    void fixedFrame();
    //固定フレーム値を設定する
    void setFixedFrame(float fixedFrame);

private:
    //フレームレートを計算する
    void computeFps(float time);

private:
    std::unique_ptr<Time> mDrawUpdateTimer;
    float mFixedFrame;
    float mCurrentFPS;
    LARGE_INTEGER mFrequency;
    unsigned long long mPreviousTime;
};
