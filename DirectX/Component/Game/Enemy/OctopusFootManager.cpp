#include "OctopusFootManager.h"
#include "OctopusFoot.h"
#include <cassert>

OctopusFootManager::OctopusFootManager()
    : Component()
{
}

OctopusFootManager::~OctopusFootManager() = default;

void OctopusFootManager::start() {
    mFoots = getComponents<OctopusFoot>();
    assert(mFoots.size() == OCTOPUS_FOOT_COUNT);

    //全足共通処理
    for (unsigned i = 0; i < OCTOPUS_FOOT_COUNT; ++i) {
        auto& foot = mFoots[i];
        foot->onDestroyFoot([&](const OctopusFoot& foot) { onDestroyFoot(foot); });

        mFootAliveNumbers.emplace_back(foot->getNumber());
    }
}

const OctopusFootPtrArray& OctopusFootManager::getFoots() const {
    return mFoots;
}

bool OctopusFootManager::isFootAlive() const {
    return (mFootAliveNumbers.size() > 0);
}

void OctopusFootManager::onDestroyFoot(const OctopusFoot& foot) {
    //死亡した足の番号を除外する
    mFootAliveNumbers.remove(foot.getNumber());
}
