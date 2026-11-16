#pragma once
#include "core/Result.hpp"
#include <cstdint>
#include <string>

namespace kow::game::systems {

class GameSystems {
public:
    GameSystems() = default;

    kow::core::Result<void> update(float dt);
    [[nodiscard]] std::uint64_t update_count() const { return update_count_; }

private:
    std::uint64_t update_count_{0};
};

} // namespace kow::game::systems
