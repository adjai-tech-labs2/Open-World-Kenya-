#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <optional>
#include <fstream>
#include "core/error_handling/result.h"

namespace kow {

struct FileStats {
    uint64_t size = 0;
    bool is_directory = false;
    bool is_file = false;
    std::filesystem::file_time_type last_write;
};

class Filesystem {
public:
    static Filesystem& instance();

    void set_root(const std::filesystem::path& root) { root_ = root; }
    const std::filesystem::path& root() const { return root_; }

    std::filesystem::path resolve(const std::filesystem::path& path) const;

    Result<std::vector<uint8_t>> read_file(const std::filesystem::path& path);
    Result<std::string> read_text(const std::filesystem::path& path);
    Result<void> write_file(const std::filesystem::path& path, const void* data, size_t size);
    Result<void> write_text(const std::filesystem::path& path, std::string_view content);

    bool exists(const std::filesystem::path& path) const;
    Result<FileStats> stat(const std::filesystem::path& path) const;
    std::vector<std::filesystem::path> list_directory(const std::filesystem::path& path) const;
    Result<void> create_directory(const std::filesystem::path& path) const;
    Result<void> remove(const std::filesystem::path& path) const;

    std::filesystem::path get_absolute_path(const std::filesystem::path& relative) const;

private:
    Filesystem() = default;
    std::filesystem::path root_;
};

// RAII file reader
class FileReader {
public:
    explicit FileReader(const std::filesystem::path& path);
    ~FileReader();
    
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;
    FileReader(FileReader&& other) noexcept;
    FileReader& operator=(FileReader&& other) noexcept;

    bool is_open() const { return file_ && file_->is_open(); }
    std::ifstream& stream() { return *file_; }
    const std::ifstream& stream() const { return *file_; }
    
    size_t size();
    std::vector<uint8_t> read_all();
    std::string read_text_all();

    const std::filesystem::path& path() const { return path_; }

private:
    std::filesystem::path path_;
    std::unique_ptr<std::ifstream> file_;
};

} // namespace kow