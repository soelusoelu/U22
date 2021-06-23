#include "ModelViewerLight.h"
#include "../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../System/Window.h"

ModelViewerLight::ModelViewerLight()
    : mDirection()
    , mColor(ColorPalette::white)
{
    mDirection.setEuler(Vector3::right * 135.f);
}

ModelViewerLight::~ModelViewerLight() = default;

void ModelViewerLight::drawGUI() {
    ImGui::Text("DirectionalLight");

    auto euler = mDirection.euler();
    if (ImGuiWrapper::dragVector3("Direction", euler, 0.1f)) {
        mDirection.setEuler(euler);
    }
    ImGuiWrapper::colorEdit3("Color", mColor);

    auto lightDirDrawPos = Vector3::right * 30.f + Vector3::up * 20.f;
    Debug::renderLine(lightDirDrawPos, lightDirDrawPos + getDirection() * 3.f, ColorPalette::red);
}

Vector3 ModelViewerLight::getDirection() const {
    return Vector3::transform(Vector3::up, mDirection);
}

const Vector3& ModelViewerLight::getColor() const {
    return mColor;
}
