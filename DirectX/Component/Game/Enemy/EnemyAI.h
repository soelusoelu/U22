#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class GameObject;
class Octopus;
class EnemyMove;
class OctopusFootAttack;

class EnemyAI
    : public Component
{
public:
    EnemyAI();
    ~EnemyAI();
    EnemyAI(const EnemyAI&) = delete;
    EnemyAI& operator=(const EnemyAI&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    void setPlayer(const std::shared_ptr<GameObject>& player);
    void onSetPlayer(const std::function<void(const std::shared_ptr<GameObject>&)>& f);

private:
    //足が一本でも生きているときの更新
    void updateFootAlive();
    //スケールを考慮した攻撃範囲を取得する
    float getAttackRange() const;
    //攻撃可能角度か
    bool canAttackAngle() const;

private:
    std::shared_ptr<GameObject> mPlayer;
    std::shared_ptr<Octopus> mOctopus;
    std::shared_ptr<EnemyMove> mMove;
    std::shared_ptr<OctopusFootAttack> mAttack;
    Function<void(const std::shared_ptr<GameObject>&)> mOnSetPlayer;
    float mAttackRange;
    float mAttackAngle;
    bool mIsDebugMode;
};
