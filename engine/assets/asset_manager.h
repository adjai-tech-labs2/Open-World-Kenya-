#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "core/error_handling/result.h"
#include "graphics/resource_handle.h"

namespace kow {

struct AssetHeader {
    uint32_t magic = 0x4B4F5731; // "KOW1"
    uint32_t version = 1;
    uint64_t asset_type_hash = 0;
    uint64_t data_size = 0;
    uint64_t metadata_size = 0;
};

enum class AssetType : uint32_t {
    Unknown = 0,
    Texture = 1,
    Mesh = 2,
    Model = 3,
    Material = 4,
    Audio = 5,
    Script = 6,
    Animation = 7,
    World = 8
};

struct AssetInfo {
    std::string path;
    AssetType type = AssetType::Unknown;
    uint64_t size = 0;
    bool is_loaded = false;
    bool is_streaming = false;
};

class IAssetLoader {
public:
    virtual ~IAssetLoader() = default;
    virtual bool can_load(AssetType type, const std::filesystem::path& path) const = 0;
    virtual Result<std::vector<uint8_t>> load_raw(AssetType type, const std::filesystem::path& path) = 0;
    virtual std::string_view name() const = 0;
};

class AssetManager {
public:
    static AssetManager& instance();

    void add_loader(std::unique_ptr<IAssetLoader> loader);
    void remove_loader(const std::string& name);

    template<typename T>
    Result<T> load_asset(const std::filesystem::path& path);

    template<typename T>
    T* get_asset(const std::string& path);

    bool is_loaded(const std::string& path) const;
    void unload_asset(const std::string& path);
    void unload_all();

    AssetInfo get_info(const std::string& path) const;
    std::vector<std::string> get_loaded_assets() const;
    void update();

    void set_cache_size(size_t max_entries);
    size_t cache_size() const { return cache_.size(); }
    size_t max_cache_size() const { return max_cache_size_; }

private:
    AssetManager();
    std::vector<std::unique_ptr<IAssetLoader>> loaders_;
    std::unordered_map<std::string, std::vector<uint8_t>> cache_;
    std::unordered_map<std::string, AssetInfo> info_;
    size_t max_cache_size_ = 256;
};

#define KOW_ASSET_MANAGER kow::AssetManager::instance()

} // namespace kow