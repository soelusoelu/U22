#include "EnemyAnimationController.h"
#include "EnemyMotions.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Math/Math.h"

EnemyAnimationController::EnemyAnimationController()
    : Component()
    , mAnimation(nullptr)
{
}

EnemyAnimationController::~EnemyAnimationController() = default;

void EnemyAnimationController::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->changeMotion(EnemyMotions::WALK);

    //色を赤に変更
    auto mesh = getComponent<MeshComponent>()->getMesh();
    Material mat = mesh->getMaterial(0);
    mat.diffuse = ColorPalette::red;
    mesh->setMaterial(mat, 0);
}
