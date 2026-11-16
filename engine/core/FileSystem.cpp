#include "FileSystem.hpp"
#include <filesystem>

namespace kow::core {

bool FileSystem::exists(const std::string& path) const {
    std::error_code ec;
    return std::filesystem::exists(path, ec);
}

bool FileSystem::is_directory(const std::string& path) const {
    std::error_code ec;
    return std::filesystem::is_directory(path, ec);
}

} // namespace kow::core
