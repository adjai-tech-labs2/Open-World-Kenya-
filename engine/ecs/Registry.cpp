#include "Registry.hpp"

namespace kow::ecs {

EntityId Registry::create() {
    std::uint32_t index;
    if (!free_indices_.empty()) {
        index = free_indices_.back();
        free_indices_.pop_back();
    } else {
        index = static_cast<std::uint32_t>(generations_.size());
        generations_.push_back(0);
    }
    // increment generation; ensure non-zero (0 reserved for dead/free in storage checks)
    auto& gen = generations_[index];
    ++gen;
    if (gen == 0) ++gen; // wrap-around guard
    ++alive_count_;
    return EntityId{index, gen};
}

void Registry::destroy(EntityId id) {
    if (!alive(id)) return;
    for (auto& [_, storage] : storages_) {
        storage->remove(id);
    }
    free_indices_.push_back(id.index);
    --alive_count_;
    // generation remains; next create will increment making old ids stale
}

bool Registry::alive(EntityId id) const {
    if (!id.valid()) return false;
    if (id.index >= generations_.size()) return false;
    return generations_[id.index] == id.generation && id.generation != 0;
}

} // namespace kow::ecs
