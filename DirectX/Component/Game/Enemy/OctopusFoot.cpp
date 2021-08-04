﻿#include "OctopusFoot.h"
#include "OctopusFootCommonSetting.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../../Utility/JsonHelper.h"
#include <algorithm>

OctopusFoot::OctopusFoot()
    : Component()
    , mFootNumber(-1)
    , mHp(0)
{
}

OctopusFoot::~OctopusFoot() = default;

void OctopusFoot::start() {
    auto commonSettings = getComponent<OctopusFootCommonSetting>();
    mHp = commonSettings->mHp;

    //全コライダーから対象ボーンのコライダーを抜き出す
    auto colliders = getComponents<OBBCollider>();
    for (const auto& c : colliders) {
        const auto no = c->getBone().number;
        const auto result = std::find(mTargetBoneNo.begin(), mTargetBoneNo.end(), no);
        if (result != mTargetBoneNo.end()) {
            mColliders.emplace_back(c);
        }
    }
}

void OctopusFoot::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mTargetBoneNo, "targetBoneNo", inObj, alloc, mode);
}

void OctopusFoot::setNumber(int number) {
    mFootNumber = number;
}

int OctopusFoot::getNumber() const {
    return mFootNumber;
}

const std::vector<std::shared_ptr<OBBCollider>>& OctopusFoot::getColliders() const {
    return mColliders;
}

void OctopusFoot::takeDamage() {
    --mHp;

    if (isDestroyFoot()) {
        //足死亡
        destroyFoot();
        mOnDestroyFoot(*this);
    }
}

bool OctopusFoot::isDestroyFoot() const {
    return (mHp <= 0);
}

void OctopusFoot::onDestroyFoot(const std::function<void(const OctopusFoot&)>& f) {
    mOnDestroyFoot += f;
}

void OctopusFoot::destroyFoot() {
    for (const auto& c : mColliders) {
        c->disabled();
    }
}
