#include "KenyaRegistry.hpp"

namespace kow::game::kenya {

kow::core::Result<void> KenyaRegistry::register_vehicle_type(const KenyanVehicleType& type) {
    if (type.name.empty()) {
        return kow::core::Err("kenya.invalid_vehicle", "Vehicle type name must not be empty");
    }
    if (vehicle_types_.find(type.id) != vehicle_types_.end()) {
        return kow::core::Err("kenya.vehicle_exists", "Vehicle type id already registered");
    }
    vehicle_types_.emplace(type.id, type);
    return kow::core::Ok();
}

const KenyanVehicleType* KenyaRegistry::vehicle_type(std::uint32_t id) const {
    auto it = vehicle_types_.find(id);
    return it == vehicle_types_.end() ? nullptr : &it->second;
}

} // namespace kow::game::kenya
