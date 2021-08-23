#pragma once

#include "../../Component.h"
#include "../../Engine/EngineComponentAlias.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class SpriteComponent;
class Time;

class YouDied
    : public Component
{
public:
    YouDied();
    ~YouDied();
    YouDied(const YouDied&) = delete;
    YouDied& operator=(const YouDied&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //画像をアクティブにする
    void activate();
    //死亡画像の描画が終了しているか
    bool hasEndedDrawing() const;
    //描画が終了したことを通知する
    void onEndedDrawing(const std::function<void()>& f);

private:
    //画像の透明度を設定する
    void setAlpha(SpriteComponent& target, const Time& timer);

private:
    SpritePtr mYouDied;
    SpritePtr mBackGround;
    std::unique_ptr<Time> mStartingUpToTime;
    std::unique_ptr<Time> mYouDiedTimer;
    std::unique_ptr<Time> mBackGroundTimer;
    Function<void()> mOnEndedDrawing;
};
