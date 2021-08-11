#pragma once

#include "EnemyAlias.h"
#include "../../Component.h"
#include <list>
#include <memory>
#include <vector>

class OctopusFoot;

class OctopusFootManager
    : public Component
{
public:
    OctopusFootManager();
    ~OctopusFootManager();
    OctopusFootManager(const OctopusFootManager&) = delete;
    OctopusFootManager& operator=(const OctopusFootManager&) = delete;

    virtual void start() override;

    //足をすべて取得する
    const OctopusFootPtrArray& getFoots() const;
    //一本でも足があるか
    bool isFootAlive() const;

private:
    //管理中の足が死んだら呼ばれる
    void onDestroyFoot(const OctopusFoot& foot);

private:
    OctopusFootPtrArray mFoots;
    std::list<unsigned> mFootAliveNumbers;

    inline static constexpr unsigned OCTOPUS_FOOT_COUNT = 8;
};
