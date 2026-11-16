#pragma once
#include <string>

namespace kow::core {

struct Version {
    int major{0};
    int minor{1};
    int patch{0};

    [[nodiscard]] std::string to_string() const {
        return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    }
};

inline constexpr Version kEngineVersion{0, 1, 0};

} // namespace kow::core
