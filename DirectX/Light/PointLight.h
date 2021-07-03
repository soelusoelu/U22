#pragma once

#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
#include "../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <vector>

class IMeshLoader;
class Shader;
struct Material;

struct PointLight {
    std::shared_ptr<IMeshLoader> mesh;
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Shader> shader;
    float radius;

    PointLight();
    ~PointLight();
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    void initialize();

private:
    std::string mMeshFileName;
};
