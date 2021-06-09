#pragma once

#include "EngineMode.h"
#include <functional>

class IEngineFunctionChanger {
public:
    virtual ~IEngineFunctionChanger() = default;
    virtual void callbackChangeMode(const std::function<void(EngineMode)>& f) = 0;
};
