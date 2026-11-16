#include "test_framework.hpp"
#include "game/kenya/KenyaRegistry.hpp"
#include "game/vehicles/VehicleRegistry.hpp"
#include "game/characters/CharacterRegistry.hpp"
#include "game/systems/GameSystems.hpp"

using namespace kow::game;

TEST(kenya_registry_vehicle_types) {
    kenya::KenyaRegistry reg;
    kenya::KenyanVehicleType vt{1, "Matatu 14-seater", "Matatu", kenya::VehicleCategory::Matatu, 50.0f, true};
    auto r = reg.register_vehicle_type(vt);
    ASSERT_TRUE(r.ok());
    ASSERT_EQ(reg.vehicle_type_count(), static_cast<std::size_t>(1));
    auto* got = reg.vehicle_type(1);
    ASSERT_TRUE(got != nullptr);
    ASSERT_EQ(got->category, kenya::VehicleCategory::Matatu);
    ASSERT_TRUE(got->is_public_transport);
    return true;
}

TEST(kenya_registry_duplicate_id) {
    kenya::KenyaRegistry reg;
    kenya::KenyanVehicleType vt{1, "Matatu", "Matatu", kenya::VehicleCategory::Matatu, 50.0f, true};
    ASSERT_TRUE(reg.register_vehicle_type(vt).ok());
    ASSERT_FALSE(reg.register_vehicle_type(vt).ok());
    return true;
}

TEST(vehicle_registry_specs) {
    vehicles::VehicleRegistry reg;
    vehicles::VehicleSpec spec{1, "Toyota", "Matatu", "14-seater", 1800.0f, 110.0f, 60.0f};
    ASSERT_TRUE(reg.register_spec(spec).ok());
    auto* s = reg.spec(1);
    ASSERT_TRUE(s != nullptr);
    ASSERT_EQ(s->kenyan_variant, std::string("14-seater"));
    return true;
}

TEST(character_registry) {
    characters::CharacterRegistry reg;
    characters::CharacterSpec cs{1, "Mzee Jomo", "Mzee", characters::CharacterRole::Vendor, "Eastlands"};
    ASSERT_TRUE(reg.register_character(cs).ok());
    auto* c = reg.character(1);
    ASSERT_TRUE(c != nullptr);
    ASSERT_EQ(c->role, characters::CharacterRole::Vendor);
    ASSERT_EQ(c->home_district, std::string("Eastlands"));
    return true;
}

TEST(game_systems_update) {
    systems::GameSystems gs;
    ASSERT_EQ(gs.update_count(), static_cast<std::uint64_t>(0));
    ASSERT_TRUE(gs.update(0.016f).ok());
    ASSERT_EQ(gs.update_count(), static_cast<std::uint64_t>(1));
    return true;
}
