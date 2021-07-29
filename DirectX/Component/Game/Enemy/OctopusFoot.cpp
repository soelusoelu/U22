#include "OctopusFoot.h"
#include "OctopusFootCommonSetting.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../../Utility/JsonHelper.h"

OctopusFoot::OctopusFoot()
    : Component()
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
        if (mTargetBoneNo.find(no) != mTargetBoneNo.end()) {
            mColliders.emplace_back(c);
        }
    }
}

void OctopusFoot::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::LOAD) {
        if (std::vector<int> temp; JsonHelper::getIntArray(temp, "targetBoneNo", inObj)) {
            mTargetBoneNo.insert(temp.cbegin(), temp.cend());
        }
    } else if (mode == FileMode::SAVE) {
        std::vector<int> temp(mTargetBoneNo.cbegin(), mTargetBoneNo.cend());
        JsonHelper::setIntArray(temp, "targetBoneNo", inObj, alloc);
    }
}

const std::vector<std::shared_ptr<OBBCollider>>& OctopusFoot::getColliders() const {
    return mColliders;
}

void OctopusFoot::takeDamage() {
    --mHp;

    if (mHp <= 0) {
        onDestroyFoot();
    }
}

void OctopusFoot::onDestroyFoot() {
    for (const auto& c : mColliders) {
        c->disabled();
    }
}
