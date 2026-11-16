#include "test_framework.hpp"
#include "world/nairobi/NairobiWorld.hpp"
#include "world/WorldManager.hpp"

using namespace kow::world;

TEST(nairobi_world_register_districts) {
    WorldManager wm;
    nairobi::NairobiWorld nairobi;
    auto r = nairobi.register_districts(wm);
    ASSERT_TRUE(r.ok());
    ASSERT_EQ(wm.sector_count(), static_cast<std::size_t>(7));
    ASSERT_TRUE(wm.get_sector({-2, 0}) != nullptr);
    ASSERT_TRUE(wm.get_sector({1, -1}) != nullptr);
    auto* cbd = wm.get_sector({-2, 0});
    ASSERT_TRUE(cbd != nullptr);
    ASSERT_EQ(cbd->name, std::string("Nairobi_CBD"));
    ASSERT_EQ(cbd->state, SectorState::Loaded);
    return true;
}

TEST(nairobi_district_list) {
    nairobi::NairobiWorld nairobi;
    ASSERT_EQ(nairobi.districts().size(), static_cast<std::size_t>(7));
    return true;
}
