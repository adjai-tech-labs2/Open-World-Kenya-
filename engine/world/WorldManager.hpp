#pragma once
#include "core/Result.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace kow::world {

struct SectorCoord {
    std::int32_t x{0};
    std::int32_t y{0};
    [[nodiscard]] bool operator==(const SectorCoord& o) const { return x == o.x && y == o.y; }
};

struct SectorCoordHash {
    std::size_t operator()(const SectorCoord& c) const noexcept {
        return (static_cast<std::size_t>(static_cast<std::uint32_t>(c.x)) << 32u) ^ static_cast<std::uint32_t>(c.y);
    }
};

enum class SectorState { Unloaded, Registered, Loaded };

struct Sector {
    SectorCoord coord{};
    std::string name;
    SectorState state{SectorState::Unloaded};
};

// Interface for world/sector management (streaming not implemented).
class IWorldManager {
public:
    virtual ~IWorldManager() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual kow::core::Result<void> register_sector(const Sector& sector) = 0;
    virtual kow::core::Result<void> set_sector_state(SectorCoord coord, SectorState state) = 0;
    [[nodiscard]] virtual const Sector* get_sector(SectorCoord coord) const = 0;
    [[nodiscard]] virtual std::size_t sector_count() const = 0;
};

class WorldManager final : public IWorldManager {
public:
    [[nodiscard]] std::string name() const override { return "WorldManager (logical sectors, no streaming backend)"; }
    kow::core::Result<void> register_sector(const Sector& sector) override;
    kow::core::Result<void> set_sector_state(SectorCoord coord, SectorState state) override;
    [[nodiscard]] const Sector* get_sector(SectorCoord coord) const override;
    [[nodiscard]] std::size_t sector_count() const override { return sectors_.size(); }
private:
    std::unordered_map<SectorCoord, Sector, SectorCoordHash> sectors_;
};

} // namespace kow::world
