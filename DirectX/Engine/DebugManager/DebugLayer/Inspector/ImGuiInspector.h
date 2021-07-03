#pragma once

#include "IInspector.h"
#include "../../../../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>

class GameObject;

//ImGuiを使用したインスペクター
class ImGuiInspector
    : public IInspector
{
public:
    ImGuiInspector();
    ~ImGuiInspector();

    virtual void setTarget(const std::shared_ptr<GameObject>& target) override;
    virtual float getInspectorPositionX() const override;

    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    void drawInspect() const;

private:
    ImGuiInspector(const ImGuiInspector&) = delete;
    ImGuiInspector& operator=(const ImGuiInspector&) = delete;

    void drawName(const GameObject& target) const;
    void drawTag(const GameObject& target) const;

private:
    std::weak_ptr<GameObject> mTarget;
    float mInspectorPositionX;
};
