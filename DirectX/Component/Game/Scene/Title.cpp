#include "Title.h"
#include "../Camera/GameCamera.h"
#include "../Camera/LockOn.h"
#include "../Player/PlayerWalk.h"
#include "../Player/PlayerWeapon.h"
#include "../UI/BossEnemyUIManager.h"
#include "../UI/PlayerUIManager.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectFactory.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Transform/Transform3D.h"

Title::Title() :
    Scene()
{
}

Title::~Title() = default;

void Title::awake() {
    auto player = GameObjectCreater::create("Player");
    auto weapon = GameObjectCreater::create("Weapon");
    auto boss = GameObjectCreater::create("Enemy");
    GameObjectCreater::create("Plane");
    auto playerUIManager = GameObjectCreater::create("PlayerUI");
    auto bossEnemyUIManager = GameObjectCreater::create("BossEnemyUI");

    auto camera = GameObjectCreater::create("GameCamera");

    const auto& camCompManager = camera->componentManager();
    camCompManager.getComponent<GameCamera>()->setPlayer(player);
    const auto& lockOn = camCompManager.getComponent<LockOn>();
    lockOn->setPlayer(player);
    auto enemys = gameObject().getGameObjectManager().findGameObjects("Enemy");
    lockOn->setEnemys(enemys);

    const auto& playerCompManager = player->componentManager();
    playerCompManager.getComponent<PlayerWalk>()->setILockOn(lockOn.get());
    playerCompManager.getComponent<PlayerWeapon>()->setWeapon(weapon);

    playerUIManager->componentManager().getComponent<PlayerUIManager>()->setPlayer(player);

    bossEnemyUIManager->componentManager().getComponent<BossEnemyUIManager>()->setBoss(boss);
}
