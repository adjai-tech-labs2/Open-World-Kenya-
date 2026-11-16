#pragma once

#include <string>
#include <memory>
#include <vector>
#include "core/error_handling/result.h"
#include "graphics/resource_handle.h"

namespace kow {

struct AssetHandle {
    uint64_t id = 0;
    std::string path;
    bool is_loaded = false;

    bool is_valid() const { return id != 0 && is_loaded; }
    bool operator==(const AssetHandle& o) const { return id == o.id; }
    bool operator!=(const AssetHandle& o) const { return id != o.id; }
};

class AssetHandleImpl {
public:
    static AssetHandle create(const std::string& path, uint64_t id) {
        return AssetHandle{id, path, true};
    }
    
    static AssetHandle invalid() {
        return AssetHandle{0, "", false};
    }
};

} // namespace kow