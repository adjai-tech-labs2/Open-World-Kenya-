#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "core/filesystem/filesystem.h"

namespace kow {

class IFileFormatHandler {
public:
    virtual ~IFileFormatHandler() = default;
    virtual bool can_handle(const std::filesystem::path& path) const = 0;
    virtual std::string_view format_name() const = 0;
    virtual std::vector<std::string> supported_extensions() const = 0;
};

class FileFormatRegistry {
public:
    static FileFormatRegistry& instance();

    void register_handler(std::unique_ptr<IFileFormatHandler> handler);
    void unregister_handler(const std::string& name);
    std::vector<std::string> get_supported_formats() const;
    bool can_handle(const std::filesystem::path& path) const;
    std::vector<std::string> get_extensions_for_format(const std::string& format) const;

private:
    FileFormatRegistry() = default;
    std::vector<std::unique_ptr<IFileFormatHandler>> handlers_;
};

class PathResolver {
public:
    static PathResolver& instance();

    void add_search_path(const std::filesystem::path& path, int priority = 0);
    void remove_search_path(const std::filesystem::path& path);
    std::filesystem::path resolve(const std::string& filename) const;
    std::vector<std::filesystem::path> find_all(const std::string& pattern) const;
    void clear();

    void set_data_directory(const std::filesystem::path& path) { data_dir_ = path; }
    const std::filesystem::path& data_directory() const { return data_dir_; }

private:
    PathResolver() = default;
    std::filesystem::path data_dir_;
    std::vector<std::filesystem::path> search_paths_;
};

#define KOW_PATH_RESOLVER kow::PathResolver::instance()

} // namespace kow