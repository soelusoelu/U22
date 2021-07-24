#include "Title.h"
#include "../Camera/GameCamera.h"
#include "../Camera/LockOn.h"
#include "../Enemy/EnemyAI.h"
#include "../Player/PlayerShield.h"
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
    //auto player = GameObjectCreater::create("Player");
    ////auto weapon = GameObjectCreater::create("Weapon");
    ////auto shield = GameObjectCreater::create("Shield");
    ////auto boss = GameObjectCreater::create("Enemy");
    //auto boss = GameObjectCreater::create("Octopus");
    //GameObjectCreater::create("Plane");
    //auto playerUIManager = GameObjectCreater::create("PlayerUI");
    //auto bossEnemyUIManager = GameObjectCreater::create("BossEnemyUI");

    //auto camera = GameObjectCreater::create("GameCamera");

    //const auto& camCompManager = camera->componentManager();
    //camCompManager.getComponent<GameCamera>()->setPlayer(player);
    //const auto& lockOn = camCompManager.getComponent<LockOn>();
    //lockOn->setPlayer(player);
    //auto enemys = gameObject().getGameObjectManager().findGameObjects("Enemy");
    //lockOn->setEnemys(enemys);

    //const auto& playerCompManager = player->componentManager();
    //playerCompManager.getComponent<PlayerWalk>()->setILockOn(lockOn.get());
    ////playerCompManager.getComponent<PlayerWeapon>()->setWeapon(weapon);
    ////playerCompManager.getComponent<PlayerShield>()->setShield(shield);

    ////boss->componentManager().getComponent<EnemyAI>()->setPlayer(player);

    //playerUIManager->componentManager().getComponent<PlayerUIManager>()->setPlayer(player);

    //bossEnemyUIManager->componentManager().getComponent<BossEnemyUIManager>()->setBoss(boss);
}

void Title::update() {
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        next("Title");
    }
}
