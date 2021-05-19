﻿#include "Title.h"
#include "../Camera/GameCamera.h"
#include "../Camera/LockOn.h"
#include "../Player/PlayerWalk.h"
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
    GameObjectCreater::create("Enemy");
    GameObjectCreater::create("Plane");
    auto playerUIManager = GameObjectCreater::create("PlayerUI");

    auto camera = GameObjectCreater::create("GameCamera");

    const auto& camCompManager = camera->componentManager();
    camCompManager.getComponent<GameCamera>()->setPlayer(player);
    const auto& lockOn = camCompManager.getComponent<LockOn>();
    lockOn->setPlayer(player);
    auto enemys = gameObject().getGameObjectManager().findGameObjects("Enemy");
    lockOn->setEnemys(enemys);

    auto pw = player->componentManager().getComponent<PlayerWalk>();
    pw->setILockOn(lockOn.get());

    playerUIManager->componentManager().getComponent<PlayerUIManager>()->setPlayer(player);
}
