#pragma once

#include "../../Component.h"
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

private:
    std::vector<OctopusFootPtr> mFoots;
    std::vector<unsigned> mFootNumbers;

    inline static constexpr unsigned OCTOPUS_FOOT_COUNT = 8;
};
