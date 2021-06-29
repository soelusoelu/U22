﻿#include "DirectionalLight.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Imgui/imgui.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

DirectionalLight::DirectionalLight() :
    Component(),
    mDirection(Vector3::down),
    mLightColor(Vector3::one) {
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::lateUpdate() {
    //メッシュの向きと合わせる
    mDirection = Vector3::transform(Vector3::down, transform().getRotation());
}

void DirectionalLight::loadProperties(const rapidjson::Value& inObj) {
    if (JsonHelper::getVector3(inObj, "direction", mDirection)) {
        transform().rotate(mDirection);
    }
    JsonHelper::getVector3(inObj, "color", mLightColor);
}

void DirectionalLight::drawInspector() {
    ImGuiWrapper::colorEdit3("Color", mLightColor);
}

void DirectionalLight::setDirection(const Vector3& dir) {
    mDirection = dir;
}

const Vector3& DirectionalLight::getDirection() const {
    return mDirection;
}

void DirectionalLight::setLightColor(const Vector3& color) {
    mLightColor = color;
}

const Vector3& DirectionalLight::getLightColor() const {
    return mLightColor;
}
