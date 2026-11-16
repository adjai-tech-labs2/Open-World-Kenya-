#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "core/error_handling/result.h"

namespace kow {

struct Sector {
    SectorID id;
    SectorBounds bounds;
    std::vector<uint8_t> terrain_data;
    std::vector<uint8_t> object_data;
    bool is_loaded = false;
    bool is_streaming = false;
    float load_priority = 0.0f;
};

} // namespace kow