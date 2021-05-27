#pragma once

#include "../../Component.h"

class WeaponDamage
    : public Component
{
public:
    WeaponDamage();
    ~WeaponDamage();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    int getDamage() const;

private:
    WeaponDamage(const WeaponDamage&) = delete;
    WeaponDamage& operator=(const WeaponDamage&) = delete;

private:
    int mDamage;
};
