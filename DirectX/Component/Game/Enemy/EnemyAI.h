#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;
class EnemyMove;
class EnemyAttack;

class EnemyAI
    : public Component
{
public:
    EnemyAI();
    ~EnemyAI();
    virtual void start() override;
    virtual void update() override;
    void setPlayer(const std::shared_ptr<GameObject>& player);

private:
    EnemyAI(const EnemyAI&) = delete;
    EnemyAI& operator=(const EnemyAI&) = delete;

private:
    std::shared_ptr<EnemyMove> mMove;
    std::shared_ptr<EnemyAttack> mAttack;
    std::shared_ptr<GameObject> mPlayer;
};
