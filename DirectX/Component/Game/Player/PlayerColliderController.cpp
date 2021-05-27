#include "PlayerColliderController.h"
#include "../../Engine/Collider/AABBAnimationCollider.h"

PlayerColliderController::PlayerColliderController()
    : Component()
    , mCollider(nullptr)
{
}

PlayerColliderController::~PlayerColliderController() = default;

void PlayerColliderController::start() {
}
