#include "core/types/version.h"

namespace kow {

std::string version_to_string(const EngineVersion& version) {
    std::string result = std::format("{}.{}.{}", version.major, version.minor, version.patch);
    if (!version.suffix.empty()) {
        result += "-" + version.suffix;
    }
    return result;
}

EngineVersion get_engine_version() {
    return EngineVersion{
        1, 0, 0,
        "alpha",
        "0000000",
        __DATE__ " " __TIME__
    };
}

} // namespace kow