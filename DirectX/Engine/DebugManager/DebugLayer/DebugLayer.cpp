#include "DebugLayer.h"
#include "FixedDebugInformation.h"
#include "Hierarchy.h"
#include "Inspector/ImGuiInspector.h"

DebugLayer::DebugLayer()
    : mFixedDebugInfo(std::make_unique<FixedDebugInformation>())
    , mHierarchy(std::make_unique<Hierarchy>())
    , mInspector(std::make_unique<ImGuiInspector>())
{
}

DebugLayer::~DebugLayer() = default;

void DebugLayer::saveAndLoad(rapidjson::Value & inObj, rapidjson::Document::AllocatorType & alloc, FileMode mode) {
    mFixedDebugInfo->saveAndLoad(inObj, alloc, mode);
    mHierarchy->saveAndLoad(inObj, alloc, mode);
    mInspector->saveAndLoad(inObj, alloc, mode);
}

void DebugLayer::initialize(const IGameObjectsGetter* gameObjectsGetter, const IFpsGetter* fpsGetter) {
    mFixedDebugInfo->initialize(fpsGetter);
    mHierarchy->initialize(gameObjectsGetter, mInspector.get());
}

void DebugLayer::update() {
    mHierarchy->update();
}

void DebugLayer::draw(EngineMode mode, DrawString& drawer, Matrix4& proj) const {
    if (mode == EngineMode::GAME) {
        mFixedDebugInfo->draw(drawer);
    }
    if (mode == EngineMode::GAME || mode == EngineMode::MAP_EDITOR) {
        mHierarchy->drawGameObjects(drawer);
    }

    mInspector->drawInspect();
}

FixedDebugInformation& DebugLayer::fixedDebugInfo() const {
    return *mFixedDebugInfo;
}

Hierarchy& DebugLayer::hierarchy() const {
    return *mHierarchy;
}

IInspector* DebugLayer::inspector() const {
    return mInspector.get();
}
