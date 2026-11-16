#pragma once
#include <string>

namespace kow::core {

class FileSystem {
public:
    virtual ~FileSystem() = default;
    [[nodiscard]] virtual bool exists(const std::string& path) const;
    [[nodiscard]] virtual bool is_directory(const std::string& path) const;
};

} // namespace kow::core
