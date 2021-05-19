#pragma once

#include "IPlayerMove.h"
#include "../Camera/ILockOn.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class SkinMeshComponent;

class PlayerWalk
    : public Component
{
public:
    PlayerWalk();
    ~PlayerWalk();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    //歩行処理
    void walk(IPlayerMove& playerMove);
    //歩いているか
    bool isWalking() const;
    //ILockOnを設定する
    void setILockOn(const ILockOn* lockOn);
    //歩行に移行した際のコールバック
    void callbackToWalk(const std::function<void()>& callback);

private:
    PlayerWalk(const PlayerWalk&) = delete;
    PlayerWalk& operator=(const PlayerWalk&) = delete;

    //歩行中の回転処理
    void rotate(IPlayerMove& playerMove);
    //モーション変更時のコールバック
    void onChangeMotion();

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    Function<void()> mCallbackToWalk;
    const ILockOn* mLockOn;
    float mWalkSpeed;
    bool mIsWalking;
};
