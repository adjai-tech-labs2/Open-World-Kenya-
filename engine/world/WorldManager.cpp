#include "WorldManager.hpp"

namespace kow::world {

kow::core::Result<void> WorldManager::register_sector(const Sector& sector) {
    if (sector.name.empty()) {
        return kow::core::Err("world.invalid_sector", "Sector name must not be empty");
    }
    if (sectors_.find(sector.coord) != sectors_.end()) {
        return kow::core::Err("world.sector_exists", "Sector already registered at coordinates");
    }
    Sector s = sector;
    if (s.state == SectorState::Unloaded) s.state = SectorState::Registered;
    sectors_[s.coord] = s;
    return kow::core::Ok();
}

kow::core::Result<void> WorldManager::set_sector_state(SectorCoord coord, SectorState state) {
    auto it = sectors_.find(coord);
    if (it == sectors_.end()) {
        return kow::core::Err("world.sector_not_found", "Sector not found");
    }
    it->second.state = state;
    return kow::core::Ok();
}

const Sector* WorldManager::get_sector(SectorCoord coord) const {
    auto it = sectors_.find(coord);
    return it == sectors_.end() ? nullptr : &it->second;
}

} // namespace kow::world
