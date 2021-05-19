#pragma once

#include <list>
#include <functional>

template<typename... Args>
class Subject {
public:
    Subject() = default;
    ~Subject() = default;

    void operator+=(const std::function<void(Args...)>& observer) {
        mObservers.emplace_back(observer);
    }

    void operator()(Args... args) {
        for (const auto& observer : mObservers) {
            observer(args...);
        }
    }

private:
    std::list<std::function<void(Args...)>> mObservers;
};
