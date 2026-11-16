#include "core/filesystem/filesystem.h"
#include "core/logging/logger.h"
#include <fstream>
#include <stdexcept>

namespace kow {

Filesystem& Filesystem::instance() {
    static Filesystem fs;
    return fs;
}

std::filesystem::path Filesystem::resolve(const std::filesystem::path& path) const {
    if (path.is_absolute()) return path;
    return root_ / path;
}

Result<std::vector<uint8_t>> Filesystem::read_file(const std::filesystem::path& path) {
    auto full_path = resolve(path);
    
    if (!std::filesystem::exists(full_path)) {
        return failure(ErrorCode::FileNotFound, 
            std::format("File not found: {}", full_path.string()));
    }

    std::ifstream file(full_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return failure(ErrorCode::PermissionDenied,
            std::format("Cannot open file: {}", full_path.string()));
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(static_cast<size_t>(size));
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return buffer;
    }

    return failure(ErrorCode::Unknown, "Failed to read file");
}

Result<std::string> Filesystem::read_text(const std::filesystem::path& path) {
    auto result = read_file(path);
    if (!result) return failure(result.error());
    
    return std::string(result->begin(), result->end());
}

Result<void> Filesystem::write_file(const std::filesystem::path& path, const void* data, size_t size) {
    auto full_path = resolve(path);
    
    auto dir = full_path.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }

    std::ofstream file(full_path, std::ios::binary);
    if (!file.is_open()) {
        return failure(ErrorCode::PermissionDenied,
            std::format("Cannot write to file: {}", full_path.string()));
    }

    file.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
    if (!file.good()) {
        return failure(ErrorCode::Unknown, "Failed to write file");
    }

    return success();
}

Result<void> Filesystem::write_text(const std::filesystem::path& path, std::string_view content) {
    return write_file(path, content.data(), content.size());
}

bool Filesystem::exists(const std::filesystem::path& path) const {
    return std::filesystem::exists(resolve(path));
}

Result<FileStats> Filesystem::stat(const std::filesystem::path& path) const {
    auto full_path = resolve(path);
    
    if (!std::filesystem::exists(full_path)) {
        return failure(ErrorCode::FileNotFound,
            std::format("File not found: {}", full_path.string()));
    }

    FileStats stats;
    stats.size = std::filesystem::file_size(full_path);
    stats.is_directory = std::filesystem::is_directory(full_path);
    stats.is_file = std::filesystem::is_regular_file(full_path);
    stats.last_write = std::filesystem::last_write_time(full_path);
    return stats;
}

std::vector<std::filesystem::path> Filesystem::list_directory(const std::filesystem::path& path) const {
    auto full_path = resolve(path);
    std::vector<std::filesystem::path> entries;
    
    if (!std::filesystem::exists(full_path) || !std::filesystem::is_directory(full_path)) {
        return entries;
    }

    for (const auto& entry : std::filesystem::directory_iterator(full_path)) {
        entries.push_back(entry.path());
    }
    return entries;
}

Result<void> Filesystem::create_directory(const std::filesystem::path& path) const {
    auto full_path = resolve(path);
    if (std::filesystem::exists(full_path)) {
        return success();
    }
    
    if (std::filesystem::create_directories(full_path)) {
        return success();
    }
    
    return failure(ErrorCode::Unknown, "Failed to create directory");
}

Result<void> Filesystem::remove(const std::filesystem::path& path) const {
    auto full_path = resolve(path);
    if (!std::filesystem::exists(full_path)) {
        return success();
    }
    
    if (std::filesystem::remove_all(full_path)) {
        return success();
    }
    
    return failure(ErrorCode::Unknown, "Failed to remove file");
}

std::filesystem::path Filesystem::get_absolute_path(const std::filesystem::path& relative) const {
    return std::filesystem::absolute(resolve(relative));
}

FileReader::FileReader(const std::filesystem::path& path) 
    : path_(path) {
    file_ = std::make_unique<std::ifstream>(Filesystem::instance().resolve(path), std::ios::binary);
}

FileReader::~FileReader() = default;

FileReader::FileReader(FileReader&& other) noexcept 
    : path_(std::move(other.path_)), file_(std::move(other.file_)) {}

FileReader& FileReader::operator=(FileReader&& other) noexcept {
    if (this != &other) {
        path_ = std::move(other.path_);
        file_ = std::move(other.file_);
    }
    return *this;
}

size_t FileReader::size() {
    if (!is_open()) return 0;
    auto& stream = stream();
    stream.seekg(0, std::ios::end);
    auto sz = stream.tellg();
    stream.seekg(0, std::ios::beg);
    return static_cast<size_t>(sz);
}

std::vector<uint8_t> FileReader::read_all() {
    if (!is_open()) return {};
    auto sz = size();
    std::vector<uint8_t> buffer(sz);
    stream().read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(sz));
    return buffer;
}

std::string FileReader::read_text_all() {
    auto data = read_all();
    return std::string(data.begin(), data.end());
}

} // namespace kow