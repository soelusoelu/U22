#include "CrossHair.h"
#include "../Player/PlayerColliderController.h"
#include "../UI/PlayerUIManager.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../System/Window.h"
#include "../../../Transform/Transform2D.h"
#include "../../../Utility/JsonHelper.h"

CrossHair::CrossHair()
    : Component()
    , mSprites(CROSS_HAIR_COUNT)
    , mOffset(0.f)
    , mLength(0.f)
{
}

CrossHair::~CrossHair() = default;

void CrossHair::start() {
    static const Vector2 dir[CROSS_HAIR_COUNT] = { Vector2::up, Vector2::right, Vector2::down, Vector2::left };
    const auto width = Window::standardWidth();
    const auto height = Window::standardHeight();
    const auto center = Vector2(width / 2.f, height / 2.f);
    for (int i = 0; i < CROSS_HAIR_COUNT; ++i) {
        auto& s = mSprites[i];
        s = addComponent<SpriteComponent>("SpriteComponent");
        s->setTextureFromFileName("System/pixel.png");
        auto& t = s->transform();
        t.setPosition(center + dir[i] * mOffset);
        t.setScale(Vector2::one + dir[i] * mLength);
    }

    getComponent<PlayerUIManager>()->callbackSetPlayer([&](const GameObject& player) {
        player.componentManager().getComponent<PlayerColliderController>()->onDead([&] {
            for (auto&& s : mSprites) {
                s->setActive(false);
            }
        });
    });
}

void CrossHair::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mOffset, "offset", inObj, alloc, mode);
    JsonHelper::getSet(mLength, "length", inObj, alloc, mode);
}
