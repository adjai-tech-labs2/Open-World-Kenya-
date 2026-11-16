#include "memory/allocator.h"
#include "core/logging/logger.h"
#include <cstdlib>
#include <new>

namespace kow {

MemoryTracker& MemoryTracker::instance() {
    static MemoryTracker tracker;
    return tracker;
}

void MemoryTracker::track_allocation(void* ptr, size_t size, MemoryPool pool,
                                      const std::string& tag, const std::source_location& loc) {
    if (!tracking_enabled_ || !ptr) return;
    
    std::unique_lock lock(mutex_);
    AllocationInfo info;
    info.ptr = ptr;
    info.size = size;
    info.pool = pool;
    info.tag = tag;
    info.location = loc;
    info.timestamp = std::chrono::steady_clock::now();
    
    allocations_[ptr] = info;
    
    auto& stats = pool_stats_[pool];
    stats.total_allocated += size;
    stats.current_usage += size;
    stats.allocation_count++;
    stats.active_allocations++;
    if (stats.current_usage > stats.peak_usage) {
        stats.peak_usage = stats.current_usage;
    }
}

void MemoryTracker::track_deallocation(void* ptr, size_t size) {
    if (!tracking_enabled_ || !ptr) return;
    
    std::unique_lock lock(mutex_);
    auto it = allocations_.find(ptr);
    if (it != allocations_.end()) {
        auto& stats = pool_stats_[it->second.pool];
        stats.total_freed += size;
        stats.current_usage -= size;
        stats.deallocation_count++;
        stats.active_allocations--;
        
        allocations_.erase(it);
    }
}

MemoryStats MemoryTracker::get_stats(MemoryPool pool) const {
    std::shared_lock lock(mutex_);
    auto it = pool_stats_.find(pool);
    if (it != pool_stats_.end()) return it->second;
    return MemoryStats{};
}

MemoryStats MemoryTracker::get_total_stats() const {
    std::shared_lock lock(mutex_);
    MemoryStats total;
    for (const auto& [pool, stats] : pool_stats_) {
        total.total_allocated += stats.total_allocated;
        total.total_freed += stats.total_freed;
        total.current_usage += stats.current_usage;
        total.peak_usage += stats.peak_usage;
        total.allocation_count += stats.allocation_count;
        total.deallocation_count += stats.deallocation_count;
        total.active_allocations += stats.active_allocations;
    }
    return total;
}

std::vector<AllocationInfo> MemoryTracker::get_leaks() const {
    std::shared_lock lock(mutex_);
    std::vector<AllocationInfo> leaks;
    for (const auto& [ptr, info] : allocations_) {
        leaks.push_back(info);
    }
    return leaks;
}

void MemoryTracker::reset_stats() {
    std::unique_lock lock(mutex_);
    pool_stats_.clear();
}

MemoryManager& MemoryManager::instance() {
    static MemoryManager manager;
    return manager;
}

void MemoryManager::initialize() {
    KOW_LOG_INFO("memory", "Memory manager initialized");
}

void MemoryManager::shutdown() {
    report_leaks();
}

void MemoryManager::update() {
    // Periodic memory checks
}

void* MemoryManager::allocate(size_t size, MemoryPool pool, const std::string& tag,
                              const std::source_location& loc) {
    void* ptr = std::malloc(size);
    if (!ptr) {
        KOW_LOG_FATAL("memory", "Failed to allocate {} bytes", size);
        throw std::bad_alloc();
    }
    
    MemoryTracker::instance().track_allocation(ptr, size, pool, tag, loc);
    return ptr;
}

void MemoryManager::deallocate(void* ptr, size_t size) {
    if (!ptr) return;
    MemoryTracker::instance().track_deallocation(ptr, size);
    std::free(ptr);
}

template<typename T, typename... Args>
T* MemoryManager::allocate_obj(MemoryPool pool, Args&&... args) {
    void* ptr = allocate(sizeof(T), pool, typeid(T).name());
    return new (ptr) T(std::forward<Args>(args)...);
}

MemoryStats MemoryManager::get_stats(MemoryPool pool) const {
    return MemoryTracker::instance().get_stats(pool);
}

MemoryStats MemoryManager::get_total_stats() const {
    return MemoryTracker::instance().get_total_stats();
}

void MemoryManager::report_leaks() const {
    auto leaks = MemoryTracker::instance().get_leaks();
    if (!leaks.empty()) {
        KOW_LOG_WARN("memory", "Memory leaks detected: {} allocations", leaks.size());
        for (const auto& leak : leaks) {
            KOW_LOG_WARN("memory", "  Leak: {} bytes at {} ({})",
                leak.size, leak.ptr, leak.tag);
        }
    }
}

void MemoryManager::reset_stats() {
    MemoryTracker::instance().reset_stats();
}

size_t MemoryManager::total_usage() const {
    return get_total_stats().current_usage;
}

size_t MemoryManager::peak_usage() const {
    return get_total_stats().peak_usage;
}

// Explicit template instantiation
template int* MemoryManager::allocate_obj<int>(MemoryPool, int&&);
template float* MemoryManager::allocate_obj<float>(MemoryPool, float&&);
template char* MemoryManager::allocate_obj<char>(MemoryPool, char&&);

} // namespace kow