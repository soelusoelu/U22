#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;

class EnemyMove
    : public Component
{
public:
    EnemyMove();
    ~EnemyMove();
    EnemyMove(const EnemyMove&) = delete;
    EnemyMove& operator=(const EnemyMove&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    void originalUpdate();

private:
    std::shared_ptr<GameObject> mPlayer;
    float mMoveSpeed;
    float mRotateSpeed;
};
