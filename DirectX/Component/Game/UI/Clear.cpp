#include "Clear.h"
#include "../Enemy/OctopusHead.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"

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
    }
}
