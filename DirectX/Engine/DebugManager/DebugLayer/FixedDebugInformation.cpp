#include "FixedDebugInformation.h"
#include "../../../Device/DrawString.h"
#include "../../../System/Window.h"
#include "../../../Utility/JsonHelper.h"
#include "../../../Utility/StringUtil.h"

FixedDebugInformation::FixedDebugInformation()
    : mFpsGetter(nullptr)
    , mScale(Vector2::one)
    , mFPSPos(Vector2::zero)
{
}

FixedDebugInformation::~FixedDebugInformation() = default;

void FixedDebugInformation::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::SAVE) {
        rapidjson::Value props(rapidjson::kObjectType);
        JsonHelper::setVector2(mScale, "scale", props, alloc);

        inObj.AddMember("fixedDebugInfo", props, alloc);
    } else {
        const auto& obj = inObj["fixedDebugInfo"];
        if (obj.IsObject()) {
            JsonHelper::getVector2(mScale, "scale", obj);
        }
    }
}

void FixedDebugInformation::initialize(const IFpsGetter* getter) {
    mFpsGetter = getter;
    mFPSPos = Vector2(Window::width() / 2.f, Window::height());
}

void FixedDebugInformation::draw(DrawString& drawString) const {
    auto drawPos = mFPSPos;
    std::string str = "fps";
    drawString.drawString(str, drawPos, mScale);
    drawPos.x += DrawString::WIDTH * (str.length() + 1) * mScale.x;
    drawString.drawString(StringUtil::floatToString(mFpsGetter->getFps(), 2), drawPos, mScale);
}
