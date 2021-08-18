#include "EngineFunctionManager.h"
#include "DebugManager/DebugManager.h"
#include "DebugManager/DebugLayer/DebugLayer.h"
#include "Pause/Pause.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Device/Renderer.h"

EngineFunctionManager::EngineFunctionManager()
    : FileOperator("EngineFunctionManager")
    , mDebugManager(std::make_unique<DebugManager>())
    , mPause(std::make_unique<Pause>())
{
}

EngineFunctionManager::~EngineFunctionManager() = default;

DebugManager& EngineFunctionManager::debug() const {
    return *mDebugManager;
}

IPause& EngineFunctionManager::pause() const {
    return *mPause;
}

void EngineFunctionManager::initialize(
    const IGameObjectsGetter* gameObjctsGetter,
    const IFpsGetter* fpsGetter
) {
#ifdef _DEBUG
    mDebugManager->initialize(gameObjctsGetter, fpsGetter, mPause.get());
    mPause->initialize();
#endif // _DEBUG
}

void EngineFunctionManager::preUpdateProcess() {
#ifdef _DEBUG
    mDebugManager->preUpdateProcess();
#endif // _DEBUG
}

void EngineFunctionManager::update() {
#ifdef _DEBUG
    mDebugManager->update();
    mPause->update();
#endif // _DEBUG
}

void EngineFunctionManager::draw2D(const Renderer& renderer, Matrix4& proj) const {
#ifdef _DEBUG
    mDebugManager->draw2D(renderer, proj);
#endif // _DEBUG
}

void EngineFunctionManager::drawDebug2D(Matrix4& proj) const {
#ifdef _DEBUG
    mDebugManager->drawDebug2D(proj);
#endif // _DEBUG
}

void EngineFunctionManager::draw3D(
    const Renderer& renderer,
    const Camera& camera
) const {
#ifdef _DEBUG
    mDebugManager->draw3D(renderer, camera.getViewProjection());
#endif // _DEBUG
}

void EngineFunctionManager::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mDebugManager->writeAndRead(inObj, alloc, mode);
    mPause->writeAndRead(inObj, alloc, mode);
}
