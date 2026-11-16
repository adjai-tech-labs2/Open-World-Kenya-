#include "memory/memory_stats.h"
#include "core/memory/allocator.h"
#include "core/logging/logger.h"

namespace kow {

MemoryStatsCollector& MemoryStatsCollector::instance() {
    static MemoryStatsCollector collector;
    return collector;
}

void MemoryStatsCollector::collect() {
    pool_stats_.clear();
    for (int i = 0; i <= static_cast<int>(MemoryPool::Temp); ++i) {
        auto pool = static_cast<MemoryPool>(i);
        pool_stats_.push_back(MemoryTracker::instance().get_stats(pool));
    }
}

void MemoryStatsCollector::reset() {
    pool_stats_.clear();
}

std::vector<MemoryPoolStats> MemoryStatsCollector::get_pool_stats() const {
    return pool_stats_;
}

MemoryPoolStats MemoryStatsCollector::get_total_stats() const {
    MemoryPoolStats total;
    for (const auto& stats : pool_stats_) {
        total.total_allocated += stats.total_allocated;
        total.total_freed += stats.total_freed;
        total.current_usage += stats.current_usage;
        total.peak_usage += stats.peak_usage;
        total.allocation_count += stats.allocation_count;
        total.active_allocations += stats.active_allocations;
    }
    return total;
}

uint64_t MemoryStatsCollector::total_usage() const {
    return get_total_stats().current_usage;
}

uint64_t MemoryStatsCollector::peak_usage() const {
    return get_total_stats().peak_usage;
}

std::unique_ptr<MemorySnapshot> MemorySnapshot::capture() {
    auto snapshot = std::make_unique<MemorySnapshot>();
    MemoryStatsCollector::instance().collect();
    auto total = MemoryStatsCollector::instance().get_total_stats();
    snapshot->total_usage_ = total.current_usage;
    snapshot->peak_usage_ = total.peak_usage;
    snapshot->pool_stats_ = MemoryStatsCollector::instance().get_pool_stats();
    return snapshot;
}

void MemorySnapshot::print() const {
    KOW_LOG_INFO("memory", "Memory Snapshot:");
    KOW_LOG_INFO("memory", "  Total usage: {} bytes", total_usage_);
    KOW_LOG_INFO("memory", "  Peak usage: {} bytes", peak_usage_);
    
    for (const auto& stats : pool_stats_) {
        KOW_LOG_INFO("memory", "  Pool: {} allocations, {} bytes current, {} peak",
            stats.active_allocations, stats.current_usage, stats.peak_usage);
    }
}

} // namespace kow