#pragma once

#include "../../../Math/Math.h"
#include <functional>

class ILockOn {
public:
    ~ILockOn() = default;
    virtual bool isLockOn() const = 0;
    virtual const Vector3& getLockOnTargetPosition() const = 0;
    virtual void callbackLockOn(const std::function<void()>& callback) = 0;
};
