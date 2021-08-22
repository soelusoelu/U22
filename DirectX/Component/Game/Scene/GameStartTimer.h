#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class Text;
class Time;

class GameStartTimer
    : public Component
{
public:
    GameStartTimer();
    ~GameStartTimer();
    GameStartTimer(const GameStartTimer&) = delete;
    GameStartTimer& operator=(const GameStartTimer&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //タイマーが終了しているか
    bool isEndTimer() const;
    //タイマー終了を知らせる
    void onEndTimer(const std::function<void()>& f);

private:
    void setTextScale(float scale);

private:
    std::shared_ptr<Text> mNumber;
    std::unique_ptr<Time> mCountDownTimer;
    Function<void()> mOnEndTimer;
    float mBeginScale;
    float mEndScale;
};
