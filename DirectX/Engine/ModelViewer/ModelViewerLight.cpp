#include "ModelViewerLight.h"
#include "../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../System/Window.h"
#include "../../Utility/JsonHelper.h"

ModelViewerLight::ModelViewerLight()
    : mDirection()
    , mColor(ColorPalette::white)
    , mDirectionDrawPosition()
    , mLengthDirection(0.f)
{
}

ModelViewerLight::~ModelViewerLight() = default;

void ModelViewerLight::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::SAVE) {
        rapidjson::Value props(rapidjson::kObjectType);
        JsonHelper::setQuaternion(mDirection, "direction", props, alloc);
        JsonHelper::setVector3(mColor, "color", props, alloc);
        JsonHelper::setVector3(mDirectionDrawPosition, "directionDrawPosition", props, alloc);
        JsonHelper::setFloat(mLengthDirection, "lengthDirection", props, alloc);

        inObj.AddMember("modelViewerLight", props, alloc);
    } else {
        const auto& obj = inObj["modelViewerLight"];
        if (obj.IsObject()) {
            JsonHelper::getQuaternion(mDirection, "direction", obj);
            JsonHelper::getVector3(mColor, "color", obj);
            JsonHelper::getVector3(mDirectionDrawPosition, "directionDrawPosition", obj);
            JsonHelper::getFloat(mLengthDirection, "lengthDirection", obj);
        }
    }
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
