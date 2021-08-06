#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>
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

    //識別番号を取得する
    int getNumber() const;
    //足の部分の全コライダーを取得する
    const std::vector<std::shared_ptr<OBBCollider>>& getColliders() const;
    //足にダメージを与える
    void takeDamage();
    //足が死んでいるか
    bool isDestroyFoot() const;
    //足が死んだら呼ばれる
    void onDestroyFoot(const std::function<void(const OctopusFoot&)>& f);

private:
    //足が死んだら
    void destroyFoot();

private:
    std::vector<OBBColliderPtr> mColliders;
    std::vector<int> mTargetBoneNo;
    Function<void(const OctopusFoot&)> mOnDestroyFoot;
    int mFootMeshNumber;
    int mHp;
};
