﻿#pragma once

#include "Inspector/IInspector.h"
#include "../../EngineMode.h"
#include "../../../GameObject/IGameObjectsGetter.h"
#include "../../../Math/Math.h"
#include "../../../System/FpsCounter/IFpsGetter.h"
#include "../../../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class DrawString;
class FixedDebugInformation;
class Hierarchy;
class ImGuiInspector;

class DebugLayer {
public:
    DebugLayer();
    ~DebugLayer();
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    void initialize(const IGameObjectsGetter* gameObjectsGetter, const IFpsGetter* fpsGetter);
    void update();
    void draw(EngineMode mode, DrawString& drawer, Matrix4& proj) const;
    FixedDebugInformation& fixedDebugInfo() const;
    Hierarchy& hierarchy() const;
    IInspector* inspector() const;

private:
    DebugLayer(const DebugLayer&) = delete;
    DebugLayer& operator=(const DebugLayer&) = delete;

private:
    std::unique_ptr<FixedDebugInformation> mFixedDebugInfo;
    std::unique_ptr<Hierarchy> mHierarchy;
    std::unique_ptr<ImGuiInspector> mInspector;
};
