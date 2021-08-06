#pragma once

#include "../../Component.h"
#include <list>
#include <memory>
#include <vector>

class OctopusFoot;

class OctopusFootManager
    : public Component
{
    using OctopusFootPtr = std::shared_ptr<OctopusFoot>;

public:
    OctopusFootManager();
    ~OctopusFootManager();
    OctopusFootManager(const OctopusFootManager&) = delete;
    OctopusFootManager& operator=(const OctopusFootManager&) = delete;

    virtual void start() override;
    virtual void update() override;

    //一本でも足があるか
    bool isFootAlive() const;

private:
    //管理中の足が死んだら呼ばれる
    void onDestroyFoot(const OctopusFoot& foot);

private:
    int mCurrentMaterial;
    std::vector<OctopusFootPtr> mFoots;
    std::list<unsigned> mFootAliveNumbers;

    inline static constexpr unsigned OCTOPUS_FOOT_COUNT = 8;
};
