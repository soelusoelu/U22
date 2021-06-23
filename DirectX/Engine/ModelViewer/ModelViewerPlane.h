#pragma once

#include "../IEngineFunctionChanger.h"
#include "../../Mesh/IMesh.h"
#include <memory>

class GameObject;
class MeshManager;

class ModelViewerPlane {
public:
    ModelViewerPlane();
    ~ModelViewerPlane();
    void initialize(
        MeshManager& meshManager,
        IEngineFunctionChanger* modeChanger
    );
    void update();
    void drawGUI();
    void onChangeModel(const IMesh& newModel);

private:
    ModelViewerPlane(const ModelViewerPlane&) = delete;
    ModelViewerPlane& operator=(const ModelViewerPlane&) = delete;

    void onModeChange(EngineMode mode);

private:
    std::shared_ptr<GameObject> mPlane;
    bool mIsDraw;
};
