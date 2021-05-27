#pragma once

#include "../../Component.h"

class EnemyAnimationController
    : public Component
{
public:
    EnemyAnimationController();
    ~EnemyAnimationController();

private:
    EnemyAnimationController(const EnemyAnimationController&) = delete;
    EnemyAnimationController& operator=(const EnemyAnimationController&) = delete;

private:

};
