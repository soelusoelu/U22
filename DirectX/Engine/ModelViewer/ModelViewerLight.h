#pragma once

#include "../../Math/Math.h"

class ModelViewerLight {
public:
    ModelViewerLight();
    ~ModelViewerLight();
    void drawGUI();
    Vector3 getDirection() const;
    const Vector3& getColor() const;

private:
    ModelViewerLight(const ModelViewerLight&) = delete;
    ModelViewerLight& operator=(const ModelViewerLight&) = delete;

private:
    Quaternion mDirection;
    Vector3 mColor;
};
