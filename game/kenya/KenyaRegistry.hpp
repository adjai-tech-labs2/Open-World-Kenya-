#pragma once
#include "core/Result.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace kow::game::kenya {

enum class VehicleCategory { Matatu, BodaBoda, Taxi, Bus, Truck, Personal };

struct KenyanVehicleType {
    std::uint32_t id{0};
    std::string name;
    std::string swahili_name;
    VehicleCategory category{VehicleCategory::Personal};
    float base_fare_kes{0.0f};
    bool is_public_transport{false};
};

class KenyaRegistry {
public:
    kow::core::Result<void> register_vehicle_type(const KenyanVehicleType& type);
    [[nodiscard]] const KenyanVehicleType* vehicle_type(std::uint32_t id) const;
    [[nodiscard]] std::size_t vehicle_type_count() const { return vehicle_types_.size(); }

private:
    std::unordered_map<std::uint32_t, KenyanVehicleType> vehicle_types_;
};

} // namespace kow::game::kenya
