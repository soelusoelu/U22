#include "CrossHair.h"
#include "../Player/PlayerColliderController.h"
#include "../UI/PlayerUIManager.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../GameObject/GameObject.h"
#include "../../../System/Window.h"
#include "../../../Utility/JsonHelper.h"

CrossHair::CrossHair()
    : Component()
    , mOffset(0.f)
    , mLength(0.f)
    , mIsRender(true)
{
}

CrossHair::~CrossHair() = default;

void CrossHair::start() {
    getComponent<PlayerUIManager>()->callbackSetPlayer([&](const GameObject& player) {
        player.componentManager().getComponent<PlayerColliderController>()->onDead([&] { mIsRender = false; });
    });
}

void CrossHair::update() {
    if (!mIsRender) {
        return;
    }

    const auto width = Window::standardWidth();
    const auto height = Window::standardHeight();
    const auto center = Vector2(width / 2.f, height / 2.f);
    Debug::renderLine(center + Vector2::up * mOffset, center + Vector2::up * mLength);
    Debug::renderLine(center + Vector2::right * mOffset, center + Vector2::right * mLength);
    Debug::renderLine(center + Vector2::down * mOffset, center + Vector2::down * mLength);
    Debug::renderLine(center + Vector2::left * mOffset, center + Vector2::left * mLength);
}

void CrossHair::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mOffset, "offset", inObj, alloc, mode);
    JsonHelper::getSet(mLength, "length", inObj, alloc, mode);
}
