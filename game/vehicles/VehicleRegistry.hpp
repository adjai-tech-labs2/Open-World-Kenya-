#pragma once
#include "core/Result.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace kow::game::vehicles {

struct VehicleSpec {
    std::uint32_t id{0};
    std::string make;
    std::string model;
    std::string kenyan_variant; // e.g. "Matatu 14-seater", "Boda boda"
    float mass_kg{1200.0f};
    float max_speed_kmh{120.0f};
    float fuel_tank_litres{50.0f};
};

class VehicleRegistry {
public:
    kow::core::Result<void> register_spec(const VehicleSpec& spec);
    [[nodiscard]] const VehicleSpec* spec(std::uint32_t id) const;
    [[nodiscard]] std::size_t spec_count() const { return specs_.size(); }

private:
    std::unordered_map<std::uint32_t, VehicleSpec> specs_;
};

} // namespace kow::game::vehicles
