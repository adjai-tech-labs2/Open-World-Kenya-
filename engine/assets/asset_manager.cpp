#include "assets/asset_manager.h"
#include "core/logging/logger.h"
#include "core/filesystem/filesystem.h"

namespace kow {

AssetManager& AssetManager::instance() {
    static AssetManager manager;
    return manager;
}

AssetManager::AssetManager() {
    loaders_.reserve(16);
    cache_.reserve(256);
}

void AssetManager::add_loader(std::unique_ptr<IAssetLoader> loader) {
    loaders_.push_back(std::move(loader));
}

void AssetManager::remove_loader(const std::string& name) {
    loaders_.erase(
        std::remove_if(loaders_.begin(), loaders_.end(),
            [&](const auto& l) { return std::string(l->name()) == name; }),
        loaders_.end()
    );
}

bool AssetManager::is_loaded(const std::string& path) const {
    return cache_.find(path) != cache_.end();
}

void AssetManager::unload_asset(const std::string& path) {
    auto it = cache_.find(path);
    if (it != cache_.end()) {
        cache_.erase(it);
        info_.erase(path);
    }
}

void AssetManager::unload_all() {
    cache_.clear();
    info_.clear();
}

AssetInfo AssetManager::get_info(const std::string& path) const {
    auto it = info_.find(path);
    if (it != info_.end()) return it->second;
    return AssetInfo{};
}

std::vector<std::string> AssetManager::get_loaded_assets() const {
    std::vector<std::string> paths;
    for (const auto& [path, _] : cache_) {
        paths.push_back(path);
    }
    return paths;
}

void AssetManager::update() {
    // Evict old assets if cache exceeds max size
    while (cache_.size() > max_cache_size_) {
        auto it = cache_.begin();
        cache_.erase(it);
    }
}

void AssetManager::set_cache_size(size_t max_entries) {
    max_cache_size_ = max_entries;
}

} // namespace kow