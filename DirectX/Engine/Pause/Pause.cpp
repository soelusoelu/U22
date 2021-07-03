#include "Pause.h"
#include "../../Device/Button.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"
#include "../../Utility/JsonHelper.h"

Pause::Pause()
    : mButton(nullptr)
    , mFileName("")
    , mOffset(Vector2::zero)
    , mIsPausing(false)
{
}

Pause::~Pause() = default;

bool Pause::isPausing() const {
    return mIsPausing;
}

void Pause::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::SAVE) {
        rapidjson::Value props(rapidjson::kObjectType);
        JsonHelper::setString(mFileName, "fileName", props, alloc);
        JsonHelper::setVector2(mOffset, "offset", props, alloc);

        inObj.AddMember("pause", props, alloc);
    } else {
        const auto& obj = inObj["pause"];
        JsonHelper::getString(mFileName, "fileName", obj);
        JsonHelper::getVector2(mOffset, "offset", obj);
    }
}

void Pause::initialize(IEngineFunctionChanger* modeChanger) {
    auto pos = mOffset;
    pos.x += Window::width();
    mButton = std::make_unique<SpriteButton>(nullptr, mFileName, pos);

    modeChanger->callbackChangeMode([&](EngineMode mode) { onModeChange(mode); });
}

void Pause::update() {
    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return;
    }

    const auto& mousePos = Input::mouse().getMousePosition();
    if (!mButton->containsPoint(mousePos)) {
        return;
    }

    //ボタンがクリックされた
    mIsPausing = !mIsPausing;
}

void Pause::onModeChange(EngineMode mode) {
    mButton->setActive(mode == EngineMode::GAME);
}
