#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>
#include <vector>

class GameObject;
class Camera;
class OctopusFoot;

class BulletShooter
    : public Component
{
public:
    BulletShooter();
    ~BulletShooter();
    BulletShooter(const BulletShooter&) = delete;
    BulletShooter& operator=(const BulletShooter&) = delete;

    virtual void start() override;
    virtual void update() override;

    void setEnemy(const GameObject& enemy);
    void ads();
    bool isAds() const;
    void onAds(const std::function<void()>& f);
    void onStartAds(const std::function<void()>& f);
    void onStopAds(const std::function<void()>& f);

private:
    std::shared_ptr<Camera> mCamera;
    std::vector<std::shared_ptr<OctopusFoot>> mEnemyFoots;
    Function<void()> mOnAds;
    Function<void()> mOnStartAds;
    Function<void()> mOnStopAds;
    bool mIsADS;
};
