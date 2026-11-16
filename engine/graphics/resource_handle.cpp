#include "graphics/resource_handle.h"
#include "core/logging/logger.h"
#include <unordered_map>

namespace kow {

ResourceManager& ResourceManager::instance() {
    static ResourceManager manager;
    return manager;
}

ResourceManager::ResourceManager() {
    resources_.reserve(256);
    loaders_.reserve(16);
}

void ResourceManager::add_loader(std::unique_ptr<IResourceLoader> loader) {
    loaders_.push_back(std::move(loader));
}

ResourceHandle ResourceManager::load_resource(const std::filesystem::path& path) {
    auto it = path_to_handle_.find(std::filesystem::absolute(path).string());
    if (it != path_to_handle_.end()) {
        return it->second;
    }

    for (auto& loader : loaders_) {
        if (loader->can_load(path)) {
            auto result = loader->load(path);
            if (result) {
                auto* resource = result->get();
                uint64_t id = next_id_++;
                ResourceHandle handle{id};
                resource->load();
                
                path_to_handle_[std::filesystem::absolute(path).string()] = handle;
                resources_.push_back(std::move(*result));
                return handle;
            }
        }
    }

    KOW_LOG_ERROR("resource", "Failed to load resource: {}", path.string());
    return ResourceHandle{0};
}

void ResourceManager::unload_resource(ResourceHandle handle) {
    if (!handle.is_valid()) return;
    
    for (auto& resource : resources_) {
        if (resource->handle().id() == handle.id()) {
            resource->unload();
            return;
        }
    }
}

void ResourceManager::unload_all() {
    for (auto& resource : resources_) {
        resource->unload();
    }
    resources_.clear();
    path_to_handle_.clear();
}

IResource* ResourceManager::get_resource(ResourceHandle handle) {
    if (!handle.is_valid()) return nullptr;
    for (auto& resource : resources_) {
        if (resource->handle().id() == handle.id()) {
            return resource.get();
        }
    }
    return nullptr;
}

const IResource* ResourceManager::get_resource(ResourceHandle handle) const {
    if (!handle.is_valid()) return nullptr;
    for (const auto& resource : resources_) {
        if (resource->handle().id() == handle.id()) {
            return resource.get();
        }
    }
    return nullptr;
}

bool ResourceManager::is_loaded(ResourceHandle handle) const {
    auto* resource = get_resource(handle);
    return resource && resource->is_loaded();
}

ResourceStats ResourceManager::get_stats(ResourceHandle handle) const {
    auto* resource = get_resource(handle);
    return resource ? resource->stats() : ResourceStats{};
}

void ResourceManager::update() {
    for (auto& resource : resources_) {
        if (resource->stats().state == ResourceState::Loading) {
            // Check if loading completed
        }
    }
}

size_t ResourceManager::resource_count() const {
    return resources_.size();
}

} // namespace kow