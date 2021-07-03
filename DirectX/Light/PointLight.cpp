#include "PointLight.h"
#include "../DirectX/InputElement.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
#include "../System/AssetsManager.h"
#include "../System/Shader/Shader.h"
#include "../Utility/JsonHelper.h"

PointLight::PointLight() :
    shader(nullptr),
    radius(0.f),
    mMeshFileName("") {
}

PointLight::~PointLight() = default;

void PointLight::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::SAVE) {
        rapidjson::Value props(rapidjson::kObjectType);
        JsonHelper::setString(mMeshFileName, "pointLightMeshFileName", props, alloc);

        inObj.AddMember("pointLight", props, alloc);
    } else {
        const auto& obj = inObj["pointLight"];
        JsonHelper::getString(mMeshFileName, "pointLightMeshFileName", obj);
    }
}

void PointLight::initialize() {
    //mesh = World::instance().assetsManager().createMeshLoader(mMeshFileName);
    //mesh->setInitMaterials(&materials);
    //radius = mesh->getRadius();
    //shader = World::instance().assetsManager().createShader("PointLight.hlsl");
}
