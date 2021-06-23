#include "ModelViewerLight.h"
#include "../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../System/Window.h"
#include "../../Utility/LevelLoader.h"

ModelViewerLight::ModelViewerLight()
    : mDirection()
    , mColor(ColorPalette::white)
    , mDirectionDrawPosition()
    , mLengthDirection(0.f)
{
}

ModelViewerLight::~ModelViewerLight() = default;

void ModelViewerLight::loadProperties(const rapidjson::Value& inObj) {
    const auto& obj = inObj["modelViewerLight"];
    if (obj.IsObject()) {
        JsonHelper::getQuaternion(obj, "direction", &mDirection);
        JsonHelper::getVector3(obj, "color", &mColor);
        JsonHelper::getVector3(obj, "directionDrawPosition", &mDirectionDrawPosition);
        JsonHelper::getFloat(obj, "lengthDirection", &mLengthDirection);
    }
}

void ModelViewerLight::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setQuaternion(alloc, &props, "direction", mDirection);
    JsonHelper::setVector3(alloc, &props, "color", mColor);
    JsonHelper::setVector3(alloc, &props, "directionDrawPosition", mDirectionDrawPosition);
    JsonHelper::setFloat(alloc, &props, "lengthDirection", mLengthDirection);

    inObj.AddMember("modelViewerLight", props, alloc);
}

void ModelViewerLight::drawGUI() {
    ImGui::Text("DirectionalLight");

    auto euler = mDirection.euler();
    if (ImGuiWrapper::dragVector3("Direction", euler, 0.1f)) {
        mDirection.setEuler(euler);
    }
    ImGuiWrapper::colorEdit3("Color", mColor);

    Debug::renderLine(mDirectionDrawPosition, mDirectionDrawPosition + getDirection() * mLengthDirection, ColorPalette::red);
}

Vector3 ModelViewerLight::getDirection() const {
    return Vector3::transform(Vector3::up, mDirection);
}

const Vector3& ModelViewerLight::getColor() const {
    return mColor;
}
