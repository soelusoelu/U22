﻿#include "FixedDebugInformation.h"
#include "../../../Device/DrawString.h"
#include "../../../System/Window.h"
#include "../../../Utility/JsonHelper.h"
#include "../../../Utility/StringUtil.h"

FixedDebugInformation::FixedDebugInformation()
    : FileOperator("FixedDebugInformation")
    , mFpsGetter(nullptr)
    , mScale(Vector2::one)
    , mFPSPos(Vector2::zero)
{
}

FixedDebugInformation::~FixedDebugInformation() = default;

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

void FixedDebugInformation::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mScale, "scale", inObj, alloc, mode);
}
