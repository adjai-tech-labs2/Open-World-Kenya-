#pragma once

#include <memory>
#include <vector>
#include <atomic>
#include "core/memory/allocator.h"

namespace kow {

struct MemoryPoolStats {
    uint64_t total_allocated = 0;
    uint64_t total_freed = 0;
    uint64_t current_usage = 0;
    uint64_t peak_usage = 0;
    uint32_t allocation_count = 0;
    uint32_t active_allocations = 0;
};

class MemoryStatsCollector {
public:
    static MemoryStatsCollector& instance();

    void collect();
    void reset();
    std::vector<MemoryPoolStats> get_pool_stats() const;
    MemoryPoolStats get_total_stats() const;
    uint64_t total_usage() const;
    uint64_t peak_usage() const;

private:
    MemoryStatsCollector() = default;
    std::vector<MemoryPoolStats> pool_stats_;
};

class MemorySnapshot {
public:
    static std::unique_ptr<MemorySnapshot> capture();
    void print() const;
    uint64_t total_usage() const { return total_usage_; }
    uint64_t peak_usage() const { return peak_usage_; }

private:
    uint64_t total_usage_ = 0;
    uint64_t peak_usage_ = 0;
    std::vector<MemoryPoolStats> pool_stats_;
};

#define KOW_MEMORY_SNAPSHOT kow::MemorySnapshot::capture()

} // namespace kow