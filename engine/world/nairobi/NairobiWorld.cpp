#include "NairobiWorld.hpp"
#include "core/Result.hpp"

namespace kow::world::nairobi {

NairobiWorld::NairobiWorld() {
    districts_ = {
        {{-2, 0}, "Nairobi_CBD", "Central Business District - financial and government hub"},
        {{-1, 1}, "Westlands", "Commercial and entertainment district"},
        {{0, 1}, "Kilimani", "Residential and commercial area"},
        {{-1, -1}, "Eastlands", "East Nairobi residential and industrial"},
        {{1, -1}, "Kibera", "Large informal settlement"},
        {{1, 0}, "Industrial_Area", "Manufacturing and logistics zone"},
        {{0, -1}, "Langata", "Residential area near Nairobi National Park"},
    };
}

kow::core::Result<void> NairobiWorld::register_districts(WorldManager& wm) {
    for (const auto& d : districts_) {
        Sector s{d.coord, d.name, SectorState::Registered};
        auto r = wm.register_sector(s);
        if (!r.ok()) {
            return r;
        }
        (void)wm.set_sector_state(d.coord, SectorState::Loaded);
    }
    return kow::core::Ok();
}

} // namespace kow::world::nairobi
