#pragma once

#include <string>
#include <cstdint>

namespace kow {

struct EngineVersion {
    uint16_t major = 1;
    uint16_t minor = 0;
    uint16_t patch = 0;
    std::string suffix;
    std::string build_hash;
    std::string build_date;
};

inline std::string version_to_string(const EngineVersion& version) {
    std::string result = std::format("{}.{}.{}", version.major, version.minor, version.patch);
    if (!version.suffix.empty()) {
        result += "-" + version.suffix;
    }
    return result;
}

inline EngineVersion get_engine_version() {
    return EngineVersion{
        1, 0, 0,
        "alpha",
        "0000000",
        __DATE__ " " __TIME__
    };
}

} // namespace kow