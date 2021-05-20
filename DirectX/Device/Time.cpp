﻿#include "Time.h"
#include "../System/SystemInclude.h"

Time::Time() :
    mCurrentTimer(0.f),
    mLimitTime(0.f) {
}

Time::~Time() = default;

void Time::update() {
    mCurrentTimer += Time::deltaTime;
}

void Time::reset() {
    mCurrentTimer = 0.f;
}

bool Time::isTime() const {
    return (mCurrentTimer >= mLimitTime);
}

void Time::setLimitTime(float sec) {
    mLimitTime = sec;
}

float Time::getLimitTime() const {
    return mLimitTime;
}

void Time::setCurrentTime(float sec) {
    mCurrentTimer = sec;
}

float Time::getCountUpTime() const {
    return mCurrentTimer;
}

float Time::getCountDownTime() const {
    return mLimitTime - mCurrentTimer;
}

float Time::rate() const {
    return mCurrentTimer / mLimitTime;
}

void Time::forceOverlimit() {
    mCurrentTimer = mLimitTime;
}

unsigned long long Time::time() {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return static_cast<unsigned long long>(time.QuadPart);
}
