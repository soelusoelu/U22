#include "Title.h"
#include "../Camera/GameCamera.h"
#include "../Camera/LockOn.h"
#include "../Player/PlayerMove.h"
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
    GameObjectCreater::create("Enemy");
    GameObjectCreater::create("Plane");

    auto camera = GameObjectCreater::create("GameCamera");

    const auto& camCompManager = camera->componentManager();
    camCompManager.getComponent<GameCamera>()->setPlayer(player);
    const auto& lockOn = camCompManager.getComponent<LockOn>();
    lockOn->setPlayer(player);
    auto enemys = gameObject().getGameObjectManager().findGameObjects("Enemy");
    lockOn->setEnemys(enemys);

    auto pm = player->componentManager().getComponent<PlayerMove>();
    pm->setILockOn(lockOn.get());
}
