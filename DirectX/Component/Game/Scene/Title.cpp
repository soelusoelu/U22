#include "Title.h"
#include "../Camera/GameCamera.h"
#include "../Camera/LockOn.h"
#include "../Camera/TPSCamera.h"
#include "../Enemy/EnemyAI.h"
#include "../Player/BulletShooter.h"
#include "../Player/PlayerWalk.h"
#include "../Player/PlayerWeapon.h"
#include "../UI/BossEnemyUIManager.h"
#include "../UI/PlayerUIManager.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectFactory.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"

Title::Title() :
    Scene()
{
}

Title::~Title() = default;

void Title::awake() {
    GameObjectCreater::create("Plane");
    auto player = GameObjectCreater::create("Player");
    //auto weapon = GameObjectCreater::create("Weapon");
    auto boss = GameObjectCreater::create("Octopus");

    auto camera = GameObjectCreater::create("TPSCamera");

    const auto& camCompManager = camera->componentManager();
    camCompManager.getComponent<TPSCamera>()->setPlayer(player);

    const auto& playerCompManager = player->componentManager();
    playerCompManager.getComponent<BulletShooter>()->setEnemy(*boss);
    playerCompManager.getComponent<PlayerWalk>()->setCameraRotation(camera->transform().getLocalRotation());
    //playerCompManager.getComponent<PlayerWeapon>()->setWeapon(weapon);

    //boss->componentManager().getComponent<EnemyAI>()->setPlayer(player);

    auto playerUIManager = GameObjectCreater::create("PlayerUI");
    playerUIManager->componentManager().getComponent<PlayerUIManager>()->setPlayer(player);

    //auto bossEnemyUIManager = GameObjectCreater::create("BossEnemyUI");
    //bossEnemyUIManager->componentManager().getComponent<BossEnemyUIManager>()->setBoss(boss);
}

void Title::update() {
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        next("Title");
    }
}
