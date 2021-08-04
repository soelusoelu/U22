#include "OctopusFootManager.h"
#include "OctopusFoot.h"
#include <cassert>

OctopusFootManager::OctopusFootManager()
    : Component()
    , mFootNumbers(OCTOPUS_FOOT_COUNT)
{
}

OctopusFootManager::~OctopusFootManager() = default;

void OctopusFootManager::start() {
    mFoots = getComponents<OctopusFoot>();
    assert(mFoots.size() == OCTOPUS_FOOT_COUNT);
}

void OctopusFootManager::update() {

}
