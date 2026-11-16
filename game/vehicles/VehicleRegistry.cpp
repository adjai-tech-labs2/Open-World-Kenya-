#include "VehicleRegistry.hpp"

namespace kow::game::vehicles {

kow::core::Result<void> VehicleRegistry::register_spec(const VehicleSpec& spec) {
    if (spec.make.empty() || spec.model.empty()) {
        return kow::core::Err("vehicle.invalid_spec", "Vehicle make and model must not be empty");
    }
    if (specs_.find(spec.id) != specs_.end()) {
        return kow::core::Err("vehicle.spec_exists", "Vehicle spec id already registered");
    }
    specs_.emplace(spec.id, spec);
    return kow::core::Ok();
}

const VehicleSpec* VehicleRegistry::spec(std::uint32_t id) const {
    auto it = specs_.find(id);
    return it == specs_.end() ? nullptr : &it->second;
}

} // namespace kow::game::vehicles
