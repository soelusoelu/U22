#include "OctopusFootManager.h"
#include "OctopusFoot.h"
#include <cassert>
#include <iterator>

OctopusFootManager::OctopusFootManager()
    : Component()
    , mFootAliveNumbers(OCTOPUS_FOOT_COUNT)
{
}

OctopusFootManager::~OctopusFootManager() = default;

void OctopusFootManager::start() {
    mFoots = getComponents<OctopusFoot>();
    assert(mFoots.size() == OCTOPUS_FOOT_COUNT);

    //全足共通処理
    for (unsigned i = 0; i < OCTOPUS_FOOT_COUNT; ++i) {
        auto& foot = mFoots[i];
        foot->setNumber(i);
        foot->onDestroyFoot([&](const OctopusFoot& foot) { onDestroyFoot(foot); });

        mFootAliveNumbers[i] = i;
    }
}

void OctopusFootManager::update() {

}

bool OctopusFootManager::isFootAlive() const {
    return (mFootAliveNumbers.size() > 0);
}

void OctopusFootManager::onDestroyFoot(const OctopusFoot& foot) {
    //死亡した足の番号を除外する
    auto res = std::find(mFootAliveNumbers.begin(), mFootAliveNumbers.end(), foot.getNumber());
    if (res != mFootAliveNumbers.end()) {
        mFootAliveNumbers.erase(res);
    }
}
