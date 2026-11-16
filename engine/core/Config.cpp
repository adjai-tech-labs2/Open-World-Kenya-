#include "Config.hpp"

namespace kow::core {

Result<void> validate_config(const EngineConfig& config) {
    if (config.app_name.empty()) {
        return Err("config.invalid_app_name", "app_name must not be empty");
    }
    if (config.target_fps <= 0.0 || config.target_fps > 1000.0) {
        return Err("config.invalid_target_fps", "target_fps must be > 0 and <= 1000");
    }
    if (config.max_frames < 0) {
        return Err("config.invalid_max_frames", "max_frames must be >= 0");
    }
    return Ok();
}

} // namespace kow::core
