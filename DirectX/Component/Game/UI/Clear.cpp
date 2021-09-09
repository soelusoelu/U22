#include "Clear.h"
#include "../Enemy/OctopusHead.h"
#include "../../Engine/Sound/SoundComponent.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Sound/Player/SoundPlayer.h"
#include "../../../Sound/Volume/SoundVolume.h"

Clear::Clear()
    : Component()
{
}

Clear::~Clear() = default;

void Clear::start() {
    mSprites = getComponents<SpriteComponent>();

    const auto& e = gameObject().getGameObjectManager().find("Enemy");
    e->componentManager().getComponent<OctopusHead>()->onDestroy([&] { setActive(true); });
}

void Clear::setActive(bool value) {
    for (const auto& s : mSprites) {
        s->setActive(value);
        auto sound = getComponent<SoundComponent>();
        sound->getSoundVolume().setVolume(0.5f);
        sound->getSoundPlayer().playStreaming();
    }
}
