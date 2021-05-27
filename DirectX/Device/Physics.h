#pragma once

#include <memory>
#include <vector>

class Collider;
class AABBCollider;
class AABBAnimationCollider;

class Physics {
    using ColliderPtr = std::shared_ptr<AABBCollider>;
    using ColliderPtrArray = std::vector<ColliderPtr>;
    using AnimationColliderPtr = std::shared_ptr<AABBAnimationCollider>;
    using AnimationColliderPtrArray = std::vector<AnimationColliderPtr>;

public:
    Physics();
    ~Physics();
    //コライダーの追加・削除
    void add(const ColliderPtr& collider);
    void add(const AnimationColliderPtr& collider);
    void remove(const ColliderPtr& collider);
    void remove(const AnimationColliderPtr& collider);
    //全削除
    void clear();
    //総当たり判定
    void sweepAndPrune();

private:
    //コライダーが有効か
    bool enabled(const Collider& collider);

private:
    ColliderPtrArray mColliders;
    AnimationColliderPtrArray mAnimationColliders;
};
