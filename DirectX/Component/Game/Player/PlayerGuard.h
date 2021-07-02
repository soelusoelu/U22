#pragma once

#include "../../Component.h"
#include "../../../Input/Input.h"
#include <memory>

class SkinMeshComponent;
class Stamina;

class PlayerGuard
    : public Component
{
public:
    PlayerGuard();
    ~PlayerGuard();
    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    void originalUpdate();
    bool isGuarding() const;

private:
    PlayerGuard(const PlayerGuard&) = delete;
    PlayerGuard& operator=(const PlayerGuard&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<Stamina> mStamina;
    bool mIsGuarding;
    float mGuardingStaminaHealRate;

    static constexpr JoyCode GUARD_BUTTON = JoyCode::LeftButton;
};
