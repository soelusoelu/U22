#pragma once

#include "../../Component.h"
#include "../../../Device/Subject.h"
#include <functional>
#include <memory>

class PlayerUIManager
    : public Component
{
    using Player = std::shared_ptr<GameObject>;

public:
    PlayerUIManager();
    ~PlayerUIManager();
    void setPlayer(const Player& player);
    const GameObject& getPlayer() const;
    void callbackSetPlayer(const std::function<void(const GameObject&)>& callback);

private:
    PlayerUIManager(const PlayerUIManager&) = delete;
    PlayerUIManager& operator=(const PlayerUIManager&) = delete;

private:
    Player mPlayer;
    Subject<const GameObject&> mCallbackSetPlayer;
};
