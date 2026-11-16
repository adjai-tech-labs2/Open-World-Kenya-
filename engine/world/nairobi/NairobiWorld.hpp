#pragma once
#include "world/WorldManager.hpp"
#include <string>
#include <vector>

namespace kow::world::nairobi {

struct NairobiDistrict {
    SectorCoord coord;
    std::string name;
    std::string description;
};

class NairobiWorld {
public:
    NairobiWorld();

    kow::core::Result<void> register_districts(WorldManager& wm);
    [[nodiscard]] const std::vector<NairobiDistrict>& districts() const { return districts_; }

private:
    std::vector<NairobiDistrict> districts_;
};

} // namespace kow::world::nairobi
