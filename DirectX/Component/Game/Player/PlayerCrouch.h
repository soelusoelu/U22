#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class Time;

class PlayerCrouch
    : public Component
{
public:
    PlayerCrouch();
    ~PlayerCrouch();
    PlayerCrouch(const PlayerCrouch&) = delete;
    PlayerCrouch& operator=(const PlayerCrouch&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    void originalUpdate();
    bool isCrouching() const;
    bool isStandingUp() const;

private:
    void crouch();
    void updateCrouch();
    void standUp();
    void updateStandUp();

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::unique_ptr<Time> mCrouchTimer;
};
