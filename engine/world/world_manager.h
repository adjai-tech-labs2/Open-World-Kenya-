#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "core/error_handling/result.h"
#include "scene/transform.h"

namespace kow {

struct SectorID {
    uint64_t x = 0;
    uint64_t z = 0;
    uint32_t lod = 0;

    bool operator==(const SectorID& o) const {
        return x == o.x && z == o.z && lod == o.lod;
    }
};

struct SectorBounds {
    Vec3 min;
    Vec3 max;
};

struct SectorData {
    SectorID id;
    SectorBounds bounds;
    std::vector<uint8_t> terrain_data;
    std::vector<uint8_t> object_data;
    bool is_loaded = false;
    bool is_streaming = false;
    float load_priority = 0.0f;
};

class ISectorProvider {
public:
    virtual ~ISectorProvider() = default;
    virtual bool can_provide(const SectorID& id) const = 0;
    virtual Result<SectorData> load_sector(const SectorID& id) = 0;
    virtual void unload_sector(const SectorID& id) = 0;
};

class WorldManager {
public:
    static WorldManager& instance();

    void initialize();
    void shutdown();
    void update(float delta_time);

    void set_sector_size(float size) { sector_size_ = size; }
    float sector_size() const { return sector_size_; }

    void set_view_distance(uint32_t distance) { view_distance_ = distance; }
    uint32_t view_distance() const { return view_distance_; }

    void set_player_position(const Vec3& position) { player_position_ = position; }
    Vec3 player_position() const { return player_position_; }

    void add_provider(std::unique_ptr<ISectorProvider> provider);
    void load_sector(const SectorID& id);
    void unload_sector(const SectorID& id);
    void unload_distant_sectors(const Vec3& viewer_position, float max_distance);

    bool is_sector_loaded(const SectorID& id) const;
    SectorData* get_sector(const SectorID& id);
    const SectorData* get_sector(const SectorID& id) const;

    size_t loaded_sector_count() const;
    std::vector<SectorID> get_loaded_sectors() const;

    SectorID world_to_sector(const Vec3& position) const;
    Vec3 sector_to_world(const SectorID& id) const;

private:
    WorldManager() = default;
    float sector_size_ = 1000.0f;
    uint32_t view_distance_ = 5;
    Vec3 player_position_{0, 0, 0};
    std::vector<std::unique_ptr<ISectorProvider>> providers_;
    std::unordered_map<uint64_t, SectorData> loaded_sectors_;
};

#define KOW_WORLD kow::WorldManager::instance()

} // namespace kow