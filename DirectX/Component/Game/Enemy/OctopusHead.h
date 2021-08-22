﻿#pragma once

#include "IOctopusPart.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>
#include <vector>

class OctopusHead
    : public Component
    , public IOctopusPart
{
public:
    OctopusHead();
    ~OctopusHead();
    OctopusHead(const OctopusHead&) = delete;
    OctopusHead& operator=(const OctopusHead&) = delete;

    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    virtual const OBBColliderPtrArray& getColliders() const override;
    virtual void takeDamage(int damage = 1) override;
    virtual int getHp() const override;
    virtual bool isDestroy() const override;

    //死亡通知を送る
    void onDestroy(const std::function<void()>& f);

private:
    OBBColliderPtrArray mColliders;
    std::vector<int> mTargetBoneNo;
    Function<void()> mOnDestroy;
    bool mIsDestroy;
};
