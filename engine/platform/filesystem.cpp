#include "platform/filesystem.h"
#include "core/logging/logger.h"
#include <algorithm>

namespace kow {

FileFormatRegistry& FileFormatRegistry::instance() {
    static FileFormatRegistry registry;
    return registry;
}

void FileFormatRegistry::register_handler(std::unique_ptr<IFileFormatHandler> handler) {
    handlers_.push_back(std::move(handler));
}

void FileFormatRegistry::unregister_handler(const std::string& name) {
    handlers_.erase(
        std::remove_if(handlers_.begin(), handlers_.end(),
            [&](const auto& h) { return std::string(h->format_name()) == name; }),
        handlers_.end()
    );
}

std::vector<std::string> FileFormatRegistry::get_supported_formats() const {
    std::vector<std::string> formats;
    for (const auto& handler : handlers_) {
        formats.push_back(std::string(handler->format_name()));
    }
    return formats;
}

bool FileFormatRegistry::can_handle(const std::filesystem::path& path) const {
    for (const auto& handler : handlers_) {
        if (handler->can_handle(path)) return true;
    }
    return false;
}

std::vector<std::string> FileFormatRegistry::get_extensions_for_format(const std::string& format) const {
    for (const auto& handler : handlers_) {
        if (std::string(handler->format_name()) == format) {
            return std::vector<std::string>(handler->supported_extensions().begin(),
                                           handler->supported_extensions().end());
        }
    }
    return {};
}

PathResolver& PathResolver::instance() {
    static PathResolver resolver;
    return resolver;
}

void PathResolver::add_search_path(const std::filesystem::path& path, int priority) {
    auto it = std::lower_bound(search_paths_.begin(), search_paths_.end(), path,
        [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return a.string() < b.string();
        });
    search_paths_.insert(it, path);
}

void PathResolver::remove_search_path(const std::filesystem::path& path) {
    search_paths_.erase(
        std::remove(search_paths_.begin(), search_paths_.end(), path),
        search_paths_.end()
    );
}

std::filesystem::path PathResolver::resolve(const std::string& filename) const {
    // Try data directory first
    if (!data_dir_.empty()) {
        auto path = data_dir_ / filename;
        if (std::filesystem::exists(path)) return path;
    }

    // Try search paths
    for (const auto& search_path : search_paths_) {
        auto path = search_path / filename;
        if (std::filesystem::exists(path)) return path;
    }

    // Try current directory
    if (std::filesystem::exists(filename)) return filename;

    return data_dir_ / filename;
}

std::vector<std::filesystem::path> PathResolver::find_all(const std::string& pattern) const {
    std::vector<std::filesystem::path> results;
    
    auto check_dir = [&](const std::filesystem::path& dir) {
        if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) return;
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.path().filename().string().find(pattern) != std::string::npos) {
                results.push_back(entry.path());
            }
        }
    };

    check_dir(data_dir_);
    for (const auto& path : search_paths_) {
        check_dir(path);
    }
    check_dir(".");

    return results;
}

void PathResolver::clear() {
    search_paths_.clear();
    data_dir_.clear();
}

} // namespace kow