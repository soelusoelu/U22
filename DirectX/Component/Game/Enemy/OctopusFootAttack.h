#pragma once

#include "EnemyAlias.h"
#include "OctopusConstantNumbers.h"
#include "../../Component.h"
#include <array>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

class SkinMeshComponent;
class OctopusFootManager;

class OctopusFootAttack
    : public Component
{
public:
    OctopusFootAttack();
    ~OctopusFootAttack();
    OctopusFootAttack(const OctopusFootAttack&) = delete;
    OctopusFootAttack& operator=(const OctopusFootAttack&) = delete;

    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    void attack();

private:
    //攻撃の種類を選択し、モーション番号を返す
    unsigned choiceAttack() const;
    //両足攻撃が可能か
    bool canDoubleAttack(const AliveNumbers& numbers, unsigned choiceNo) const;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<OctopusFootManager> mFootManager;
    //叩きつけ攻撃時のペアとなる足配列
    std::array<std::pair<unsigned, unsigned>, OctopusConstantNumbers::FOOT_COUNT / 2> mFootPairs;
    //攻撃モーションマップ
    std::unordered_map<unsigned, unsigned> mSingleFootAttackMotionMap;
    std::unordered_map<unsigned, unsigned> mDoubleFootAttackMotionMap;
    //叩きつけ攻撃の発生確率
    int mSwingDownProbability;
};
