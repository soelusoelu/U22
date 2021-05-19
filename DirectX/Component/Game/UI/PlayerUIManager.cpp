#include "PlayerUIManager.h"

PlayerUIManager::PlayerUIManager()
    : Component()
    , mPlayer(nullptr)
{
}

PlayerUIManager::~PlayerUIManager() = default;

void PlayerUIManager::setPlayer(const Player& player) {
    mPlayer = player;

    mCallbackSetPlayer(*player);
}

const GameObject& PlayerUIManager::getPlayer() const {
    return *mPlayer;
}

void PlayerUIManager::callbackSetPlayer(const std::function<void(const GameObject&)>& callback) {
    mCallbackSetPlayer += callback;
}
