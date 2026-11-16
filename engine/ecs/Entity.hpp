#pragma once
#include <cstdint>
#include <functional>
#include <limits>
#include <vector>

namespace kow::ecs {

struct EntityId {
    std::uint32_t index{std::numeric_limits<std::uint32_t>::max()};
    std::uint32_t generation{0};

    [[nodiscard]] bool operator==(const EntityId& other) const {
        return index == other.index && generation == other.generation;
    }
    [[nodiscard]] bool operator!=(const EntityId& other) const { return !(*this == other); }
    [[nodiscard]] bool valid() const { return index != std::numeric_limits<std::uint32_t>::max(); }
};

struct EntityIdHash {
    std::size_t operator()(const EntityId& id) const noexcept {
        return (static_cast<std::size_t>(id.index) << 32u) ^ static_cast<std::size_t>(id.generation);
    }
};

} // namespace kow::ecs
