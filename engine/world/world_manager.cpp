#include "world/world_manager.h"
#include "core/logging/logger.h"
#include <unordered_map>

namespace kow {

WorldManager& WorldManager::instance() {
    static WorldManager manager;
    return manager;
}

void WorldManager::initialize() {
    KOW_LOG_INFO("world", "World manager initialized");
}

void WorldManager::shutdown() {
    loaded_sectors_.clear();
    providers_.clear();
    KOW_LOG_INFO("world", "World manager shutdown");
}

void WorldManager::update(float delta_time) {
    // Update streaming based on player position
}

void WorldManager::add_provider(std::unique_ptr<ISectorProvider> provider) {
    providers_.push_back(std::move(provider));
}

void WorldManager::load_sector(const SectorID& id) {
    if (is_sector_loaded(id)) return;

    for (auto& provider : providers_) {
        if (provider->can_provide(id)) {
            auto result = provider->load_sector(id);
            if (result) {
                uint64_t key = (id.x << 32) | (id.z << 8) | id.lod;
                loaded_sectors_[key] = std::move(*result);
                KOW_LOG_DEBUG("world", "Loaded sector ({}, {}, {})", id.x, id.z, id.lod);
                return;
            }
        }
    }
}

void WorldManager::unload_sector(const SectorID& id) {
    uint64_t key = (id.x << 32) | (id.z << 8) | id.lod;
    auto it = loaded_sectors_.find(key);
    if (it != loaded_sectors_.end()) {
        for (auto& provider : providers_) {
            provider->unload_sector(id);
        }
        loaded_sectors_.erase(it);
    }
}

void WorldManager::unload_distant_sectors(const Vec3& viewer_position, float max_distance) {
    std::vector<uint64_t> to_remove;
    
    for (const auto& [key, sector] : loaded_sectors_) {
        Vec3 sector_center = sector_to_world(SectorID{
            static_cast<uint32_t>(key >> 32),
            static_cast<uint32_t>((key >> 8) & 0xFFFFFF),
            static_cast<uint32_t>(key & 0xFF)
        });
        
        float distance = (sector_center - viewer_position).length();
        if (distance > max_distance) {
            to_remove.push_back(key);
        }
    }
    
    for (uint64_t key : to_remove) {
        loaded_sectors_.erase(key);
    }
}

bool WorldManager::is_sector_loaded(const SectorID& id) const {
    uint64_t key = (id.x << 32) | (id.z << 8) | id.lod;
    return loaded_sectors_.find(key) != loaded_sectors_.end();
}

SectorData* WorldManager::get_sector(const SectorID& id) {
    uint64_t key = (id.x << 32) | (id.z << 8) | id.lod;
    auto it = loaded_sectors_.find(key);
    if (it != loaded_sectors_.end()) return &it->second;
    return nullptr;
}

const SectorData* WorldManager::get_sector(const SectorID& id) const {
    uint64_t key = (id.x << 32) | (id.z << 8) | id.lod;
    auto it = loaded_sectors_.find(key);
    if (it != loaded_sectors_.end()) return &it->second;
    return nullptr;
}

size_t WorldManager::loaded_sector_count() const {
    return loaded_sectors_.size();
}

std::vector<SectorID> WorldManager::get_loaded_sectors() const {
    std::vector<SectorID> sectors;
    for (const auto& [key, _] : loaded_sectors_) {
        sectors.push_back(SectorID{
            static_cast<uint32_t>(key >> 32),
            static_cast<uint32_t>((key >> 8) & 0xFFFFFF),
            static_cast<uint32_t>(key & 0xFF)
        });
    }
    return sectors;
}

SectorID WorldManager::world_to_sector(const Vec3& position) const {
    SectorID id;
    id.x = static_cast<uint64_t>(std::floor(position.x / sector_size_));
    id.z = static_cast<uint64_t>(std::floor(position.z / sector_size_));
    id.lod = 0;
    return id;
}

Vec3 WorldManager::sector_to_world(const SectorID& id) const {
    return Vec3{
        static_cast<float>(id.x) * sector_size_,
        0.0f,
        static_cast<float>(id.z) * sector_size_
    };
}

} // namespace kow