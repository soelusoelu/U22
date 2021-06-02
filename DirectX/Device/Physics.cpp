#include "Physics.h"
#include "../Collision/Collision.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Collider/AABBAnimationCollider.h"
#include "../Component/Engine/Collider/AABBCollider.h"
#include <algorithm>

Physics::Physics() {
    Collider::setPhysics(this);
}

Physics::~Physics() {
    Collider::setPhysics(nullptr);
}

void Physics::add(const ColliderPtr& collider) {
    mColliders.emplace_back(collider);
}

void Physics::add(const std::shared_ptr<AABBAnimationCollider>& collider) {
    mAnimationColliders.emplace_back(collider);
}

void Physics::remove(const ColliderPtr& collider) {
    auto itr = std::find(mColliders.begin(), mColliders.end(), collider);
    if (itr != mColliders.end()) {
        std::iter_swap(itr, mColliders.end() - 1);
        mColliders.pop_back();
    }
}

void Physics::remove(const AnimationColliderPtr& collider) {
    auto itr = std::find(mAnimationColliders.begin(), mAnimationColliders.end(), collider);
    if (itr != mAnimationColliders.end()) {
        std::iter_swap(itr, mAnimationColliders.end() - 1);
        mAnimationColliders.pop_back();
    }
}

void Physics::clear() {
    mColliders.clear();
}

void Physics::sweepAndPrune() {
    if (mColliders.empty()) {
        return;
    }

    //min.xが小さい順にソート
    std::sort(
        mColliders.begin(),
        mColliders.end(),
        [&](const ColliderPtr& a, const ColliderPtr& b) { return a->getAABB().min.x < b->getAABB().min.x; }
    );

    for (size_t i = 0; i < mColliders.size(); ++i) {
        const auto& aColl = mColliders[i];
        if (!enabled(*aColl)) {
            continue;
        }
        const auto& a = aColl->getAABB();

        for (size_t j = i + 1; j < mColliders.size(); ++j) {
            const auto& bColl = mColliders[j];
            if (!enabled(*bColl)) {
                continue;
            }
            const auto& b = bColl->getAABB();

            //もしb.min.xが、a.max.xを超えていたら、
            //aと交差する可能性があるbは存在しない
            if (b.min.x > a.max.x) {
                break;
            }

            //衝突判定
            if (Intersect::intersectAABB(a, b)) {
                aColl->addHitCollider(bColl);
                bColl->addHitCollider(aColl);
            }
        }
    }

    for (size_t i = 0; i < mAnimationColliders.size(); ++i) {
        auto& animCollsA = mAnimationColliders[i];
        if (!enabled(*animCollsA)) {
            continue;
        }

        for (const auto& animCollA : animCollsA->getAABBs()) {
            if (!animCollA.isActive) {
                continue;
            }

            for (const auto& coll : mColliders) {
                if (!enabled(*coll)) {
                    continue;
                }

                if (Intersect::intersectAABB(animCollA.aabb, coll->getAABB())) {
                    animCollsA->addHitCollider(coll);
                    coll->addHitCollider(animCollsA);
                }
            }



            for (size_t j = i + 1; j < mAnimationColliders.size(); ++j) {
                const auto& animCollsB = mAnimationColliders[j];
                if (!enabled(*animCollsB)) {
                    continue;
                }

                for (const auto& animCollB : animCollsB->getAABBs()) {
                    if (!animCollB.isActive) {
                        continue;
                    }

                    if (Intersect::intersectAABB(animCollA.aabb, animCollB.aabb)) {
                        animCollsA->addHitCollider(animCollsB);
                        animCollsB->addHitCollider(animCollsA);
                    }
                }

            }
        }
    }
}

bool Physics::enabled(const Collider& collider) {
    if (!collider.gameObject().getActive()) {
        return false;
    }
    if (!collider.getEnable()) {
        return false;
    }

    return true;
}
