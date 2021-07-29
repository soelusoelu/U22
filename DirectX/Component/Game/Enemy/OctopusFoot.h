#pragma once

#include "../../Component.h"
#include <memory>
#include <unordered_set>
#include <vector>

class OBBCollider;

class OctopusFoot
    : public Component
{
    using OBBColliderPtr = std::shared_ptr<OBBCollider>;

public:
    OctopusFoot();
    ~OctopusFoot();
    OctopusFoot(const OctopusFoot&) = delete;
    OctopusFoot& operator=(const OctopusFoot&) = delete;

    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //足の部分の全コライダーを取得する
    const std::vector<std::shared_ptr<OBBCollider>>& getColliders() const;
    //足にダメージを与える
    void takeDamage();

private:
    void onDestroyFoot();

private:
    std::vector<OBBColliderPtr> mColliders;
    std::unordered_set<int> mTargetBoneNo;
    int mHp;
};
