#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <atomic>
#include "core/error_handling/result.h"

namespace kow {

enum class MemoryPool : uint8_t {
    Game = 0,
    Render = 1,
    Physics = 2,
    Audio = 3,
    Asset = 4,
    Network = 5,
    UI = 6,
    Temp = 7
};

struct MemoryStats {
    uint64_t total_allocated = 0;
    uint64_t total_freed = 0;
    uint64_t current_usage = 0;
    uint64_t peak_usage = 0;
    uint32_t allocation_count = 0;
    uint32_t deallocation_count = 0;
    uint32_t active_allocations = 0;
};

struct AllocationInfo {
    void* ptr;
    size_t size;
    MemoryPool pool;
    std::string tag;
    std::source_location location;
    std::chrono::steady_clock::time_point timestamp;
};

class IMemoryAllocator {
public:
    virtual ~IMemoryAllocator() = default;
    virtual void* allocate(size_t size, MemoryPool pool, const std::string& tag = "") = 0;
    virtual void deallocate(void* ptr, size_t size) = 0;
    virtual bool owns(void* ptr) const = 0;
    virtual MemoryStats stats() const = 0;
    virtual void reset() = 0;
};

class MemoryTracker {
public:
    static MemoryTracker& instance();

    void track_allocation(void* ptr, size_t size, MemoryPool pool, 
                          const std::string& tag, const std::source_location& loc);
    void track_deallocation(void* ptr, size_t size);
    
    MemoryStats get_stats(MemoryPool pool) const;
    MemoryStats get_total_stats() const;
    std::vector<AllocationInfo> get_leaks() const;
    void reset_stats();
    void set_tracking_enabled(bool enabled) { tracking_enabled_ = enabled; }
    bool is_tracking_enabled() const { return tracking_enabled_; }

private:
    MemoryTracker() = default;
    std::unordered_map<void*, AllocationInfo> allocations_;
    std::unordered_map<MemoryPool, MemoryStats> pool_stats_;
    std::mutex mutex_;
    bool tracking_enabled_ = true;
};

class MemoryManager {
public:
    static MemoryManager& instance();

    void initialize();
    void shutdown();
    void update();

    void* allocate(size_t size, MemoryPool pool, const std::string& tag = "",
                   const std::source_location& loc = std::source_location::current());
    void deallocate(void* ptr, size_t size);

    template<typename T, typename... Args>
    T* allocate_obj(MemoryPool pool, Args&&... args);

    MemoryStats get_stats(MemoryPool pool) const;
    MemoryStats get_total_stats() const;
    void report_leaks() const;
    void reset_stats();

    size_t total_usage() const;
    size_t peak_usage() const;

private:
    MemoryManager() = default;
    std::unique_ptr<IMemoryAllocator> default_allocator_;
};

#define KOW_ALLOC(size, pool, tag) kow::MemoryManager::instance().allocate(size, pool, tag)
#define KOW_FREE(ptr, size) kow::MemoryManager::instance().deallocate(ptr, size)
#define KOW_NEW(type, pool, ...) kow::MemoryManager::instance().allocate_obj<type>(pool, ##__VA_ARGS__)

} // namespace kow