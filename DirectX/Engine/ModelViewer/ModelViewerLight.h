#pragma once

#include "../../Math/Math.h"
#include "../../Utility/FileMode.h"
#include <rapidjson/document.h>

class ModelViewerLight {
public:
    ModelViewerLight();
    ~ModelViewerLight();
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    void drawGUI();
    Vector3 getDirection() const;
    const Vector3& getColor() const;

private:
    ModelViewerLight(const ModelViewerLight&) = delete;
    ModelViewerLight& operator=(const ModelViewerLight&) = delete;

private:
    Quaternion mDirection;
    Vector3 mColor;
    Vector3 mDirectionDrawPosition;
    float mLengthDirection;
};
